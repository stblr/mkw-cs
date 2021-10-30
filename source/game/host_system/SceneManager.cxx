#include "SceneManager.hxx"

namespace System {

REPLACE void SceneManager::changeSceneWithCreator(s32 sceneId, EGG::SceneCreator *creator) {
    m_creator = creator;
    changeSiblingScene(sceneId);
}

} // namespace System
