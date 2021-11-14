#pragma once

#define REPLACE(symbol, offset) \
    .section replacements \
    .global asm ## offset # _ ## symbol $ \
    asm_ ## offset ## _ ## symbol
