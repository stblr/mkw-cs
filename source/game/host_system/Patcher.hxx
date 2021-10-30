#pragma once

#include <Common.hxx>

namespace System {
namespace Patcher {

enum class Binary {
    None,
    Dol,
    Rel,
};

struct Patch {
    Binary getBinary() const;

    void *from;
    void *to;
};

extern const u32 patchCount;
extern const Patch patches[];

void patch(Binary binary);

} // namespace Patcher
} // namespace System
