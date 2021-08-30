// SPDX-License-Identifier: MIT

// Based on:
// - di.c from BrainSlug
// https://wiibrew.org/wiki//dev/di

#include "di.h"

#include "ios.h"

enum di_ioctl {
    DI_IOCTL_READ_DISK_ID = 0x70,
    DI_IOCTL_READ = 0x71,
    DI_IOCTL_GET_COVER_STATUS = 0x88,
    DI_IOCTL_RESET = 0x8a,
    DI_IOCTL_READ_UNENCRYPTED = 0x8d,
};

enum di_ioctlv {
    DI_IOCTLV_OPEN_PARTITION = 0x8b,
};

enum di_result {
    DI_RESULT_SUCCESS = 1,
};

static const char di_ipc_path[] __attribute__((aligned(0x20))) = "/dev/di";
static u32 di_ipc_in[8] __attribute__((aligned(0x20)));
static u32 di_ipc_out[8] __attribute__((aligned(0x20)));
static struct ioctlv di_ipc_vec[5] __attribute__((aligned(0x20)));
static u8 di_ipc_tmd[0x49e4] __attribute__((aligned(0x20)));
static s32 di_fd = -1;

bool di_init(void) {
    di_fd = ios_open(di_ipc_path, 0);
    return di_fd >= 0;
}

bool di_read_disk_id(void) {
    di_ipc_in[0] = DI_IOCTL_READ_DISK_ID << 24;
    
    s32 ret = ios_ioctl(di_fd, DI_IOCTL_READ_DISK_ID, di_ipc_in, sizeof(di_ipc_in),
                        (void *)0x80000000, 0x20);

    return ret == DI_RESULT_SUCCESS;
}

bool di_read_unencrypted(void *dst, u32 size, u32 offset) {
    di_ipc_in[0] = DI_IOCTL_READ_UNENCRYPTED << 24;
    di_ipc_in[1] = size;
    di_ipc_in[2] = offset >> 2;

    s32 ret = ios_ioctl(di_fd, DI_IOCTL_READ_UNENCRYPTED, di_ipc_in, sizeof(di_ipc_in), dst, size);

    return ret == DI_RESULT_SUCCESS;
}

bool di_open_partition(u32 offset) {
    di_ipc_in[0] = DI_IOCTLV_OPEN_PARTITION << 24;
    di_ipc_in[1] = offset >> 2;

    di_ipc_vec[0].data = di_ipc_in;
    di_ipc_vec[0].size = sizeof(di_ipc_in);
    di_ipc_vec[1].data = NULL;
    di_ipc_vec[1].size = 0;
    di_ipc_vec[2].data = NULL;
    di_ipc_vec[2].size = 0;
    di_ipc_vec[3].data = di_ipc_tmd;
    di_ipc_vec[3].size = sizeof(di_ipc_tmd);
    di_ipc_vec[4].data = di_ipc_out;
    di_ipc_vec[4].size = sizeof(di_ipc_out);

    s32 ret = ios_ioctlv(di_fd, DI_IOCTLV_OPEN_PARTITION, 3, 2, di_ipc_vec);

    return ret == DI_RESULT_SUCCESS;
}

bool di_read(void *dst, u32 size, u32 offset) {
    di_ipc_in[0] = DI_IOCTL_READ << 24;
    di_ipc_in[1] = size;
    di_ipc_in[2] = offset >> 2;

    s32 ret = ios_ioctl(di_fd, DI_IOCTL_READ, di_ipc_in, sizeof(di_ipc_in), dst, size);

    return ret == DI_RESULT_SUCCESS;
}

bool di_is_inserted(void) {
    di_ipc_in[0] = DI_IOCTL_GET_COVER_STATUS << 24;

    s32 ret = ios_ioctl(di_fd, DI_IOCTL_GET_COVER_STATUS, di_ipc_in, sizeof(di_ipc_in), di_ipc_out,
                        sizeof(di_ipc_out));

    if (ret != 1) {
        return false;
    }

    return !!(di_ipc_out[0] & 2);
}

bool di_reset(void) {
    di_ipc_in[0] = DI_IOCTL_RESET << 24;
    di_ipc_in[1] = true; // Enable spinup

    s32 ret = ios_ioctl(di_fd, DI_IOCTL_RESET, di_ipc_in, sizeof(di_ipc_in), NULL, 0);

    return ret == DI_RESULT_SUCCESS;
}

bool di_fini(void) {
    bool ret = ios_close(di_fd) == 0;
    di_fd = -1;
    return ret;
}
