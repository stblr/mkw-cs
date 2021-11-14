#include "GameScene.hxx"

namespace System {

GameScene *GameScene::s_instance = nullptr;

GameScene *GameScene::createStubInstance(EGG::Heap *heap) {
    s_instance = new GameScene(heap);
    return s_instance;
}

GameScene *GameScene::my_getInstance() {
    return s_instance;
}

GameScene::GameScene(EGG::Heap *heap) {
    m_heapCollection.m_heaps[0] = heap;
    m_heapCollection.m_heaps[1] = heap;
    m_otherHeapCollection.m_heaps[0] = heap;
    m_otherHeapCollection.m_heaps[1] = heap;
}

} // namespace System

REPLACE(getInstance__Q26System9GameSceneFv);
