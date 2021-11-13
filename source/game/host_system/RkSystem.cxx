#include "RkSystem.hxx"

#include "Patcher.hxx"
#include "Rel.hxx"
#include "Scene.hxx"

#include <egg/core/eggExpHeap.hxx>

extern "C" {
    #include <rvl/os.h>
}

namespace System {

#ifdef CLIENT
void RkSystem::my_main(int argc, const char *const *argv) {
    s_instance = &s_system;
    s_parentInstance = &s_system;

    s_argc = argc;
    s_argv = argv;

    s_system.initialize();

    s_sceneCreatorStatic = new (s_system.getSystemHeap(), 0x4) SceneCreatorStatic;
    s_system.m_sceneManager->my_changeSceneWithCreator(SCENE_ID_BOOT_STRAP, s_sceneCreatorStatic);

    delete s_system.m_relHeap;

    s_system.run();
}
#endif

#ifdef SERVER
void RkSystem::my_main(int argc, const char *const *argv) {
    EGG::Heap::initialize();
    void *start = OSGetMEM1ArenaLo();
    u32 size = static_cast<u8 *>(OSGetMEM1ArenaHi()) - static_cast<u8 *>(OSGetMEM1ArenaLo());
    EGG::Heap *heap = EGG::ExpHeap::create(start, size, 0);
    heap->becomeCurrentHeap();

    Rel::load(heap);
    Patcher::patch(Patcher::Binary::Rel);
}
#endif

} // namespace System

REPLACE(main__Q26System8RkSystemFiPCPCc);
