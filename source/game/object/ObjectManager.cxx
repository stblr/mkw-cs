#include "ObjectManager.hxx"

namespace Object {

void ObjectKclManager::init() {
    for (u32 i = 0; i < m_count; i++) {
        m_objs[i].loadRoute();
    }
}

ObjectManager *ObjectManager::createInstance() {
    s_instance = new ObjectManager;
    ObjectKclManager::createInstance();
    s_instance->createObjects(1 /* Mii objects */);
    s_instance->createObjects(0 /* the rest */);
    return s_instance;
}

void ObjectManager::init() {
    ObjectKclManager::getInstance()->init();
}

} // namespace Object
