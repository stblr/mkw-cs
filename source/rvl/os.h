#pragma once

#include "gx/GXStruct.h"

void *OSAllocFromMEM1ArenaLo(u32 size, u32 align);

#define OSRoundUp32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))

void OSInit(void);

void OSFatal(GXColor fg, GXColor bg, const char* msg);
