#pragma once

#include "ItemSlot.hxx"

namespace Item {

class ItemKart {
public:
    ItemKart();
    ~ItemKart();
    void init(u32 idx);
    ItemSlot &getSlot() {
        return m_itemSlot;
    }

private:
    u8 _0x000[0x088];
    ItemSlot m_itemSlot;
    u8 _0x0b4[0x248 - 0x0b4];
};

} // namespace Item
