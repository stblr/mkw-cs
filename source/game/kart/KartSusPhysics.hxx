#pragma once

#include "KartComponent.hxx"

namespace Kart {

class KartSusPhysics : KartComponent {
public:
    KartSusPhysics(u32 tireIdx, bool xMirrored, u32 bspTireIdx);

private:
    u8 _0c[0x48 - 0x0c];
};

} // namespace Kart
