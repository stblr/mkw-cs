// SPDX-License-Identifier: MIT

#pragma once

#include "types.h"

void invalidate_dcache_range(void *start, size_t size);

void flush_dcache_range(void *start, size_t size);
