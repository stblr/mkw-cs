// SPDX-License-Identifier: MIT

#include "apploader.h"
#include "delay.h"
#include "di.h"

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

    game_entry();

    return 0;
}
