#pragma once

#include "KartComponent.hxx"

namespace Kart {

class KartPart : private KartComponent {
public:
    KartPart();
    virtual ~KartPart();
    virtual void vf_0c();
    virtual void vf_10();
    virtual void vf_14();
    virtual void vf_18();
    virtual void vf_1c();
    virtual void vf_20();
    virtual void vf_24();
    virtual void vf_28();
    virtual void vf_2c();
    virtual void vf_30();
    virtual void vf_34();
    virtual void vf_38();
    virtual void vf_3c();
    virtual void vf_40();

private:
    u8 _10[0x90 - 0x10];
};

} // namespace Kart
