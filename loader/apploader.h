// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "types.h"

typedef void (*fn_apploader_report)(const char *format, ...);
typedef void (*fn_apploader_game_entry)(void);

typedef void (*fn_apploader_init)(fn_apploader_report);
typedef s32 (*fn_apploader_main)(void **dst, u32 *size, u32 *shifted_offset);
typedef fn_apploader_game_entry (*fn_apploader_close)(void);

struct apploader {
    fn_apploader_init init;
    fn_apploader_main main;
    fn_apploader_close close;
};

bool apploader_load(struct apploader *apploader);

bool apploader_run(const struct apploader *apploader, fn_apploader_game_entry *game_entry);
