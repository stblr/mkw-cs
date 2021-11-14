#pragma once

#include "KartComponent.hxx"

namespace Kart {

class KartTirePhysics : public KartComponent {
public:
    void init();

private:
    u8 _0c[0x84 - 0x0c];
};

} // namespace Kart
