// SPDX-License-Identifier: MIT

#include "types.h"

#include <string.h>

void *memset(void *s, int c, size_t n) {
    u8 *p = s;
    for (size_t i = 0; i < n; i++) {
        p[i] = c;
    }
    return s;
}
