#pragma once

#include <Common.hxx>

namespace System {
namespace Patcher {

enum class Binary {
    None,
    Dol,
    Rel,
};

void patch(Binary binary);

} // namespace Patcher
} // namespace System
