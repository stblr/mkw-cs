#include "ItemObj.hxx"

namespace Item {

void ItemObj::my_init(u16 globalId, u16 localId, ItemObjId objId) {
    m_objId = objId;
    m_globalId = globalId;
    m_localId = globalId;
    _074 = 0;
    _078 = 0;
    _07c = 0;
    // TODO weird object collision stuff
}

} // namespace Item

REPLACE(init__Q24Item7ItemObjFUsUsQ24Item9ItemObjId);
