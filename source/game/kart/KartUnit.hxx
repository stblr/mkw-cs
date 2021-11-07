#pragma once

#include "KartComponent.hxx"

namespace Kart {

class KartParams;

struct KartAccessor {
    KartParams *params;
    u8 _04[0x64 - 0x04];
};

class KartUnit : public KartComponent {
private:
    KartUnit();
    virtual ~KartUnit();
    virtual void vf_0c();
    virtual void vf_10();
    virtual void initTires();
    virtual void vf_18();
    void init();
    void createTires();
    void createSub(); // TODO better name
    void createGraphics(); // TODO better name
    void initTirePhysics();

    u8 _10[0x14 - 0x10];
    KartParams *m_params;
    u8 _18[0x1c - 0x18];
    KartAccessor m_accessor;
};

} // namespace Kart
