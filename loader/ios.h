// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "types.h"

struct ioctlv {
	void *data;
	u32 size;
};

s32 ios_open(const char *path, u32 mode);

s32 ios_close(s32 fd);

s32 ios_ioctl(s32 fd, u32 ioctl, void *in_buf, size_t in_size, void *out_buf, size_t out_size);

s32 ios_ioctlv(s32 fd, u32 ioctlv, size_t in_count, size_t out_count, struct ioctlv *vec);
