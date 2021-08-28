// SPDX-License-Identifier: GPL-2.0-or-later

// Based on:
// - VolumeWii.cpp and Boot_BS2Emu.cpp from Dolphin
// - apploader.c from BrainSlug
// - https://wiibrew.org/wiki/Apploader
// - https://www.gc-forever.com/wiki/index.php?title=Apploader

#include "apploader.h"

struct partition_group {
    u32 count;
    u32 offset_shifted;
};

struct partition {
    u32 offset_shifted;
    u32 type;
};

enum partition_type {
    PARTITION_TYPE_DATA              = 0,
    PARTITION_TYPE_UPDATE            = 1,
    PARTITION_TYPE_CHANNEL_INSTALLER = 2,
};

typedef void (*fn_apploader_entry)(fn_apploader_init *init, fn_apploader_main *main,
                                   fn_apploader_close *close);

struct apploader_hdr {
    char revision[16];
    fn_apploader_entry entry;
    u32 size;
    u32 trailer;
    u8 _pad[4];
};

static bool find_game_partition(struct partition *game_partition) {
    struct partition_group groups[4] __attribute__((aligned(0x20)));
    if (!di_read_unencrypted(groups, sizeof(groups), 0x40000)) {
        return false;
    }

    for (u32 group_idx = 0; group_idx < 4; group_idx++) {
        u32 partition_count = groups[group_idx].count;
        u32 offset = groups[group_idx].offset_shifted << 2;

        if (partition_count == 0 || partition_count > 4 || offset == 0) {
            continue;
        }

        struct partition partitions[4] __attribute__((aligned(0x20)));
        if (!di_read_unencrypted(partitions, sizeof(partitions), offset)) {
            return false;
        }

        for (u32 partition_idx = 0; partition_idx < partition_count; partition_idx++) {
            if (partitions[partition_idx].type == PARTITION_TYPE_DATA) {
                *game_partition = partitions[partition_idx];
                return true;
            }
        }
    }

    return false;
}

bool apploader_load(struct apploader *apploader) {
    if (!di_init()) {
        return false;
    }

    if (!di_read_disk_id()) {
        return false;
    }

    struct partition game_partition;
    if (!find_game_partition(&game_partition)) {
        return false;
    }

    if (!di_open_partition(game_partition.offset_shifted << 2)) {
        return false;
    }

    struct apploader_hdr hdr __attribute__((aligned(0x20)));
    if (!di_read(&hdr, sizeof(hdr), 0x2440)) {
        return false;
    }

    if (!di_read((void *)0x81200000, hdr.size + hdr.trailer, 0x2460)) {
        return false;
    }

    hdr.entry(&apploader->init, &apploader->main, &apploader->close);

    return true;
}

static void report(const char *format, ...) {
    (void)format;
    // Nop
}

bool apploader_run(const struct apploader *apploader, fn_apploader_game_entry *game_entry) {
    apploader->init(report);

    void *dst;
    u32 size;
    u32 shifted_offset;
    while (apploader->main(&dst, &size, &shifted_offset) != 0) {
        if (!di_read(dst, size, shifted_offset << 2)) {
            return false;
        }
    }

    *game_entry = apploader->close();

    return true;
}
