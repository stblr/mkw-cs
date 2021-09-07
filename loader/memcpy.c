// SPDX-License-Identifier: MIT

#include "types.h"

#include <string.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    u8 *d = dest;
    const u8 *s = src;

    while (n-- > 0) {
        *d++ = *s++;
    }

    return dest;
}
