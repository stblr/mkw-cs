#include "ItemManager.hxx"

#include "ItemObjProperties.hxx"
#include "ItemPathManager.hxx"
#include "ItemProperties.hxx"
#include "ItemSlotTable.hxx"

#include "../race/RaceGlobals.hxx"

namespace Item {

ItemManager *ItemManager::createInstance() {
    s_instance = new ItemManager;
    return s_instance;
}

ItemManager::ItemManager() {
    m_kartCount = Race::RaceGlobals::kartCount();

    ItemProperties::init();
    ItemObjProperties::init();
    ItemSlotTable::createInstance();
    ItemPathManager::createInstance();
    // TODO 8079f8b4 needed or not?

    u32 globalId = 0;
    for (u32 objId = 0; objId < static_cast<u32>(ItemObjId::Max); objId++) {
        m_objDirectors[objId].init(static_cast<ItemObjId>(objId), globalId);
    }

    m_karts = new ItemKart[m_kartCount];
    /*for (u32 i = 0; i < m_kartCount; i++) {
        m_karts[i].init(i);
    }*/

    _42c = 0;
}

void ItemManager::init() {
    if (Race::RaceGlobals::isTimeAttack()) {
        for (u32 i = 0; i < m_kartCount; i++) {
            m_karts[i].getSlot().setItem(ItemId::Kinoko3, false);
        }
    }

    initItemDirect();
}

} // namespace Item
