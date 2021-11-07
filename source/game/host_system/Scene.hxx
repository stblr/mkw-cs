#pragma once

#include <egg/core/eggScene.hxx>

namespace System {

enum SceneId {
    SCENE_ID_BOOT_STRAP = 0x5,
};

class HeapCollection {
public:
    void setGroupIdAll(s32 groupId);

    EGG::Heap *m_heaps[3];

private:
    u8 _00c[0xc0c - 0x00c];
};

class Scene : public EGG::Scene {
public:
    ~Scene() override;

protected:
    HeapCollection m_heapCollection;

private:
    u8 _c3c[0xc70 - 0xc3c];
};

}
