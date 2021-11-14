#pragma once

#include "../host_system/Scene.hxx"

namespace System {

class GameScene : public Scene {
public:
    static GameScene *createStubInstance(EGG::Heap *heap);
    static GameScene *my_getInstance();

    ~GameScene() override;
    void calc() override;
    void draw() override;
    void enter() override;
    void exit() override;

private:
    GameScene(EGG::Heap *heap);

    static GameScene *s_instance;

    u8 _0c70[0x0c94 - 0x0c70];
    HeapCollection m_otherHeapCollection;
    u8 _18a0[0x254c - 0x18a0];
};

} // namespace System
