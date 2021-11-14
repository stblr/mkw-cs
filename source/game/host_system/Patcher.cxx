#include "Patcher.hxx"

#include "Dol.hxx"
#include "Rel.hxx"

extern "C" {
    #include <rvl/os/OSCache.h>
}

namespace System {
namespace Patcher {

extern "C" const Patch __start_patches;
extern "C" const Patch __stop_patches;

static Binary getBinary(void *address) {
    if (address >= Dol::getStart() && address < Dol::getEnd()) {
        return Binary::Dol;
    }

    if (address >= Rel::getStart() && address < Rel::getEnd()) {
        return Binary::Rel;
    }

    return Binary::None;
}

void patch(Binary binary) {
    const Patch *patches = &__start_patches;
    u32 patchCount = &__stop_patches - &__start_patches;

    for (u32 i = 0; i < patchCount; i++) {
        if (getBinary(patches[i].fromBase) != binary) {
            continue;
        }

        u8 *from = patches[i].fromBase + patches[i].fromOffset;
        u8 *to = patches[i].toBase + patches[i].toOffset;

        s32 diff = to - from;
        u32 ins = 0x12 << 26 | diff;
        *reinterpret_cast<u32 *>(from) = ins;

        DCFlushRange(from, sizeof(u32));
        ICInvalidateRange(from, sizeof(u32));
    }
}

} // namespace Patcher
} // namespace System
