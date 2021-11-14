#pragma once

#include "KartPart.hxx"
#include "KartTirePhysics.hxx"

namespace Kart {

class KartTire : public KartPart {
public:
    virtual void createPhysics(u32 tireIdx);
    void init(u32 tireIdx, void *r5);

private:
    u8 _90[0x98 - 0x90];
    KartTirePhysics *m_physics;
    u8 _9c[0xa0 - 0x9c];
};

} // namespace Kart
