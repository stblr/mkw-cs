#include "SceneCreatorStatic.hxx"

#include "BootStrapScene.hxx"
#include "Scene.hxx"

namespace System {

REPLACE EGG::Scene *SceneCreatorStatic::create(s32 sceneId) {
    switch (sceneId) {
    case SCENE_ID_BOOT_STRAP:
        return new BootStrapScene;
    default:
        return nullptr;
    }
}

REPLACE void SceneCreatorStatic::destroy(s32 sceneId) {}

} // namespace System
