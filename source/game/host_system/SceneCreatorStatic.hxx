#pragma once

#include <egg/core/eggSceneManager.hxx>

namespace System {

class SceneCreatorStatic : public EGG::SceneCreator {
public:
    EGG::Scene *create(s32 sceneId) override;

    void destroy(s32 sceneId) override;
};

} // namespace System
