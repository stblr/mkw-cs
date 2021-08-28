// SPDX-License-Identifier: MIT

#include "apploader.h"
#include "delay.h"

static bool run(void) {
    struct apploader apploader;
    if (!apploader_load(&apploader)) {
        return false;
    }

    fn_apploader_game_entry game_entry;
    if (!apploader_run(&apploader, &game_entry)) {
        return false;
    }

    game_entry();

    return true;
}

int main(void) {
    while (!run()) {
        while (!di_is_inserted()) {
            mdelay(1);
        }

        di_reset();
        mdelay(1);
    }

    return 0;
}
