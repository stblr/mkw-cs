#pragma once

#include "../host_system/Scene.hxx"

namespace System {

class RootScene : public Scene {
public:
    static RootScene *createStubInstance(EGG::Heap *heap);
    static RootScene *getInstance() {
        return s_instance;
    }

    ~RootScene() override;
    void calc() override;
    void draw() override;
    void enter() override;
    void exit() override;

private:
    RootScene(EGG::Heap *heap);

    static RootScene *s_instance;

    u8 _c70[0xcb8 - 0xc70];
};

} // namespace System
