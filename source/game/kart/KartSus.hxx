#pragma once

#include "KartPart.hxx"
#include "KartSusPhysics.hxx"

namespace Kart {

class KartSus : public KartPart {
public:
    virtual void vf_44();
    void init(void *r4, u32 tireIdx, bool xMirrored, u32 bspTireIdx);

private:
    KartSusPhysics *m_physics;
};

} // namespace Kart
