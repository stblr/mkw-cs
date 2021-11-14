#pragma once

#include "KartPart.hxx"

namespace Kart {

class KartBody : public KartPart {
public:
    virtual void vf_44();
    virtual void vf_48();
    virtual void vf_4c();
    virtual void vf_50();
    virtual void vf_54();
    virtual void vf_58();
    virtual void vf_5c();
    virtual void vf_60();
    void init();

private:
    u8 _090[0x234 - 0x090];
};

} // namespace Kart
