#pragma once

#include "ItemId.hxx"

namespace Item {

class ItemSlot {
public:
    void setItem(ItemId itemId, bool unk);

private:
    u8 _00[0x2c];
};

} // namespace Item
