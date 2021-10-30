#pragma once

#include "eggScene.hxx"

namespace EGG {

class SceneCreator {
public:
    virtual Scene *create(s32 sceneId) = 0;

    virtual void destroy(s32 sceneId) = 0;
};

class SceneManager {
public:
    virtual void calc();

    virtual void draw();

    virtual void calcCurrentScene();

    virtual void calcCurrentFader();

    virtual void drawCurrentScene();

    virtual void drawCurrentFader();

    virtual void createDefaultFader();

    void changeSiblingScene(s32 sceneId);

protected:
    SceneCreator *m_creator;

private:
    u8 _08[0x2c - 0x08];
};

} // namespace EGG
