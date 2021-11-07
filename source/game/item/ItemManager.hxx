#pragma once

#include "ItemKart.hxx"
#include "ItemObj.hxx"
#include "ItemObjDirector.hxx"

#include <egg/core/eggDisposer.hxx>

namespace Item {

class ItemManager : public EGG::Disposer {
public:
    static ItemManager *createInstance();

    virtual ~ItemManager() override;
    void init();
    void initItemDirect();

private:
    ItemManager();

    static ItemManager *s_instance;

    u8 m_kartCount;
    ItemKart *m_karts;
    u8 _018[0x048 - 0x018];
    ItemObjDirector m_objDirectors[15];
    u8 _264[0x42c - 0x264];
    u32 _42c;
};

} // namespace Item
