#include "RkSystem.hxx"

#include "Scene.hxx"

namespace System {

REPLACE void RkSystem::main(int argc, const char *const *argv) {
    s_instance = &s_system;
    s_parentInstance = &s_system;

    s_argc = argc;
    s_argv = argv;

    s_system.initialize();

    s_sceneCreatorStatic = new (s_system.getSystemHeap(), 0x4) SceneCreatorStatic;
    s_system.m_sceneManager->changeSceneWithCreator(SCENE_ID_BOOT_STRAP, s_sceneCreatorStatic);

    delete s_system.m_relHeap;

    s_system.run();
}

} // namespace System
