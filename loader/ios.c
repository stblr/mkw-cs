// SPDX-License-Identifier: GPL-2.0-or-later

// Based on:
// - WII_IPC.cpp from Dolphin
// - ios.c from The Homebrew Channel
// - https://wiibrew.org/wiki/IOS
// - https://wiibrew.org/wiki/Hardware/IPC

#include "ios.h"

#include "cache.h"

#include <string.h>

enum hw_ipc_reg {
    HW_IPC_PPCMSG  = 0xcd000000,
    HW_IPC_PPCCTRL = 0xcd000004,
    HW_IPC_ARMMSG  = 0xcd000008,
};

enum ppcctrl_bit {
    X1  = 1 << 0,
    Y2  = 1 << 1,
    Y1  = 1 << 2,
    X2  = 1 << 3,
    IY1 = 1 << 4,
    IY2 = 1 << 5,
};

enum ipc_cmd_type {
    IPC_CMD_OPEN   = 1,
    IPC_CMD_CLOSE  = 2,
    IPC_CMD_READ   = 3,
    IPC_CMD_WRITE  = 4,
    IPC_CMD_SEEK   = 5,
    IPC_CMD_IOCTL  = 6,
    IPC_CMD_IOCTLV = 7,
    IPC_REPLY      = 8,
};

static u32 ipc_read(u32 reg) {
    return *(volatile u32 *)reg;
}

static void ipc_write(u32 reg, u32 val) {
    *(volatile u32 *)reg = val;
}

struct ipc {
    u32 cmd;
    s32 result;
    s32 fd;
    u32 arg[5];
    u32 user[8];
};

static struct ipc ipc __attribute__((aligned(0x40)));

static u32 mem_virtual_to_physical(void *ptr) {
    return (u32)ptr & 0x7fffffff;
}

static void *mem_physical_to_virtual(u32 addr) {
    return (void *)(addr | 0x80000000);
}

static void ipc_send_request(void) {
    flush_dcache_range(&ipc, 0x40);

    ipc_write(HW_IPC_PPCMSG, mem_virtual_to_physical(&ipc));
    ipc_write(HW_IPC_PPCCTRL, X1);

    while ((ipc_read(HW_IPC_PPCCTRL) & Y2) != Y2);
    ipc_write(HW_IPC_PPCCTRL, Y2);
}

static void ipc_recv_reply(void) {
    u32 reply;
    do {
        while ((ipc_read(HW_IPC_PPCCTRL) & Y1) != Y1);
        reply = ipc_read(HW_IPC_ARMMSG);
        ipc_write(HW_IPC_PPCCTRL, Y1);

        ipc_write(HW_IPC_PPCCTRL, X2);
    } while (reply != mem_virtual_to_physical(&ipc));

    invalidate_dcache_range(&ipc, 0x40);
}

s32 ios_open(const char *path, u32 mode) {
    flush_dcache_range((void *)path, strlen(path) + 1);

    memset(&ipc, 0, sizeof(ipc));

    ipc.cmd = IPC_CMD_OPEN;
    ipc.fd = 0;
    ipc.arg[0] = mem_virtual_to_physical((void *)path);
    ipc.arg[1] = mode;

    ipc_send_request();
    ipc_recv_reply();

    return ipc.result;
}

s32 ios_close(s32 fd) {
    memset(&ipc, 0, sizeof(ipc));

    ipc.cmd = IPC_CMD_CLOSE;
    ipc.fd = fd;

    ipc_send_request();
    ipc_recv_reply();

    return ipc.result;
}

s32 ios_ioctl(s32 fd, u32 ioctl, void *in_buf, size_t in_size, void *out_buf, size_t out_size) {
    flush_dcache_range(in_buf, in_size);
    flush_dcache_range(out_buf, out_size);

    memset(&ipc, 0, sizeof(ipc));

    ipc.cmd = IPC_CMD_IOCTL;
    ipc.fd = fd;
    ipc.arg[0] = ioctl;
    ipc.arg[1] = mem_virtual_to_physical(in_buf);
    ipc.arg[2] = in_size;
    ipc.arg[3] = mem_virtual_to_physical(out_buf);
    ipc.arg[4] = out_size;

    ipc_send_request();
    ipc_recv_reply();

    invalidate_dcache_range(out_buf, out_size);

    return ipc.result;
}

s32 ios_ioctlv(s32 fd, u32 ioctlv, size_t in_count, size_t out_count, struct ioctlv *vec) {
    for (u32 i = 0; i < in_count + out_count; i++) {
        if (vec[i].data) {
            flush_dcache_range(vec[i].data, vec[i].size);
            vec[i].data = (void *)mem_virtual_to_physical(vec[i].data);
        }
    }
    flush_dcache_range(vec, (in_count + out_count) * sizeof(struct ioctlv));

    memset(&ipc, 0, sizeof(ipc));

    ipc.cmd = IPC_CMD_IOCTLV;
    ipc.fd = fd;
    ipc.arg[0] = ioctlv;
    ipc.arg[1] = in_count;
    ipc.arg[2] = out_count;
    ipc.arg[3] = mem_virtual_to_physical(vec);

    ipc_send_request();
    ipc_recv_reply();

    for (u32 i = in_count; i < in_count + out_count; i++) {
        if (vec[i].data) {
            vec[i].data = mem_physical_to_virtual((u32)vec[i].data);
            invalidate_dcache_range(vec[i].data, vec[i].size);
        }
    }

    return ipc.result;
}
