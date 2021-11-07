#include "ItemObj.hxx"

namespace Item {

REPLACE void ItemObj::init(u16 globalId, u16 localId, ItemObjId objId) {
    m_objId = objId;
    m_globalId = globalId;
    m_localId = globalId;
    _074 = 0;
    _078 = 0;
    _07c = 0;
    // TODO weird object collision stuff
}

} // namespace Item
