#pragma once

#include "Rel.hxx"

#include <egg/core/eggScene.hxx>

namespace System {

class BootStrapScene : public EGG::Scene {
public:
    BootStrapScene();

    ~BootStrapScene() override;

    void calc() override;

    void enter() override;

    void exit() override;

private:
    Rel::Loader *m_relLoader;
};

} // namespace System
