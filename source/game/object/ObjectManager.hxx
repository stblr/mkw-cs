#pragma once

#include "ObjectKcl.hxx"

#include <Common.hxx>

namespace Object {

class ObjectKclManager {
public:
    static ObjectKclManager *createInstance();
    static ObjectKclManager *getInstance() {
        return s_instance;
    }
    void init();

private:
    static ObjectKclManager *s_instance;

    u8 _00[0x04];
    u16 m_count;
    ObjectKcl *m_objs;
    u8 _0c[0x20 - 0x0c];
};

class ObjectManager {
public:
    static ObjectManager *createInstance();
    void init();

private:
    ObjectManager();
    void createObjects(u8 category);

    static ObjectManager *s_instance;

    u8 _000[0x160];
};

} // namespace Object
