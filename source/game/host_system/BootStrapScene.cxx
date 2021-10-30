#include "BootStrapScene.hxx"

#include "Patcher.hxx"

namespace System {

BootStrapScene::BootStrapScene() : m_relLoader(nullptr) {}

BootStrapScene::~BootStrapScene() {}

void BootStrapScene::calc() {
    Rel::EntryFunction entry = m_relLoader->poke();
    if (entry) {
        Patcher::patch(Patcher::Binary::Rel);

        entry();
    }
}

void BootStrapScene::enter() {
    m_relLoader = new (m_heapMem1, 0x4) Rel::Loader(m_heapMem1);
}

void BootStrapScene::exit() {
    delete m_relLoader;
}

} // namespace System
