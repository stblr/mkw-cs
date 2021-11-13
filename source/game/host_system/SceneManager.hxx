#pragma once

#include <egg/core/eggSceneManager.hxx>

namespace System {

class SceneManager : public EGG::SceneManager {
public:
    void my_changeSceneWithCreator(s32 sceneId, EGG::SceneCreator *creator);
};

} // namespace System
