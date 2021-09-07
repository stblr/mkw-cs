// SPDX-License-Identifier: MIT

#include "apploader.h"
#include "cache.h"
#include "delay.h"
#include "di.h"
#include "patches.h"

#include <string.h>

int main(void) {
    while (!di_init()) {
        mdelay(100);
    }

    fn_apploader_game_entry game_entry;
    while (!apploader_load_and_run(&game_entry)) {
        mdelay(100);

        if (di_is_inserted()) {
            di_reset();
        }
    }

    di_fini();

    u32 offset = 0;
    while (offset < patches_size) {
        void *start = *(void **)(patches + offset);
        offset += 4;

        u32 size = *(u32 *)(patches + offset);
        offset += 4;

        memcpy(start, patches + offset, size);
        flush_dcache_range(start, size);
        invalidate_icache_range(start, size);

        offset += size;
    }

    game_entry();

    return 0;
}
