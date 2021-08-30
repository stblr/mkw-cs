// SPDX-License-Identifier: MIT

#pragma once

#include "types.h"

bool di_init(void);

bool di_read_disk_id(void);

bool di_read_unencrypted(void *dst, u32 size, u32 offset);

bool di_open_partition(u32 offset);

bool di_read(void *dst, u32 size, u32 offset);

bool di_is_inserted(void);

bool di_reset(void);

bool di_fini(void);
