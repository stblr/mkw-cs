// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "types.h"

typedef void (*fn_apploader_game_entry)(void);

bool apploader_load_and_run(fn_apploader_game_entry *game_entry);
