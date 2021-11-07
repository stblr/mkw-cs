#pragma once

#include <Common.hxx>

namespace Item {

enum class ItemObjId : u32 {
    KouraMidori = 0x0,
    KouraAka = 0x1,
    Banana = 0x2,
    Kinoko = 0x3,
    Star = 0x4,
    KouraAo = 0x5,
    Thunder = 0x6,
    DummyBox = 0x7,
    KinokoBig = 0x8,
    Bomb = 0x9,
    Gesso = 0xa,
    Pow = 0xb,
    KinokoBolden = 0xc,
    Killer = 0xd,
    Kumo = 0xe,
    Max,
};

class ItemObj {
public:

    ItemObj();
    void init(u16 globalId, u16 localId, ItemObjId);

private:
    u8 _000[0x004];
    ItemObjId m_objId;
    u16 m_globalId;
    u16 m_localId;
    u8 _00c[0x074 - 0x00c];
    u32 _074;
    u32 _078;
    u32 _07c;
    u8 _080[0x180 - 0x080];
};

} // namespace Item
