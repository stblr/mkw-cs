#pragma once

#include "ItemObj.hxx"

namespace Item {

class ItemObjDirector {
public:
    ItemObjDirector();
    ~ItemObjDirector();
    void init(ItemObjId objId, u32 &globalId);

private:
    u8 _00[0x24];
};

} // namespace Item
