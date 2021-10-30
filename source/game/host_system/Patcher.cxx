#include "Patcher.hxx"

#include "Dol.hxx"
#include "Rel.hxx"

extern "C" {
    #include <rvl/os/OSCache.h>
}

namespace System {
namespace Patcher {

Binary Patch::getBinary() const {
    if (from >= Dol::getStart() && from < Dol::getEnd()) {
        return Binary::Dol;
    }

    if (from >= Rel::getStart() && from < Rel::getEnd()) {
        return Binary::Rel;
    }

    return Binary::None;
}

void patch(Binary binary) {
    for (u32 i = 0; i < patchCount; i++) {
        if (patches[i].getBinary() != binary) {
            continue;
        }

        s32 from = reinterpret_cast<s32>(patches[i].from);
        s32 to = reinterpret_cast<s32>(patches[i].to);
        s32 diff = to - from;
        u32 ins = 0x12 << 26 | diff;
        *reinterpret_cast<u32 *>(patches[i].from) = ins;

        DCFlushRange(patches[i].from, sizeof(u32));
        ICInvalidateRange(patches[i].from, sizeof(u32));
    }
}

} // namespace Patcher
} // namespace System
