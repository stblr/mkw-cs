#include "RootScene.hxx"

namespace System {

RootScene *RootScene::createStubInstance(EGG::Heap *heap) {
    s_instance = new RootScene(heap);
    return s_instance;
}

RootScene::RootScene(EGG::Heap *heap) {
    m_heapCollection.m_heaps[0] = heap;
    m_heapCollection.m_heaps[1] = heap;
}

} // namespace System
