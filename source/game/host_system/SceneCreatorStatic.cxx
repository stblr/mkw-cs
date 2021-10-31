#include "SceneCreatorStatic.hxx"

#include "BootStrapScene.hxx"
#include "Scene.hxx"

namespace System {

EGG::Scene *SceneCreatorStatic::create(s32 sceneId) {
    switch (sceneId) {
    case SCENE_ID_BOOT_STRAP:
        return new BootStrapScene;
    default:
        return nullptr;
    }
}

void SceneCreatorStatic::destroy(s32 sceneId) {}

} // namespace System
