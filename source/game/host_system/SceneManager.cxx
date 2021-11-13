#include "SceneManager.hxx"

namespace System {

void SceneManager::my_changeSceneWithCreator(s32 sceneId, EGG::SceneCreator *creator) {
    m_creator = creator;
    changeSiblingScene(sceneId);
}

} // namespace System

REPLACE(changeSceneWithCreator__Q26System12SceneManagerFlPQ23EGG12SceneCreator);
