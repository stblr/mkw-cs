// SPDX-License-Identifier: MIT

#include <game/host_system/SceneCreatorStatic.hpp>

#include "CsBootStrapScene.hpp"

namespace System {

EGG::Scene *SceneCreatorStatic::create(int sceneId) {
    switch (sceneId) {
    case RK_SCENE_ID_BOOTSTRAP:
        return new CsBootStrapScene();
    default:
        return nullptr;
    }
}

void SceneCreatorStatic::destroy(int sceneId) {}

} // namespace System
