#pragma once

#include "SceneCreatorStatic.hxx"
#include "SceneManager.hxx"

namespace System {

// Don't bother with the full inheritance and templates for now
class RkSystem {
public:
    static void main(int argc, const char *const *argv);

    virtual void vf_08();
    virtual EGG::Heap *getSystemHeap();
    virtual void vf_10();
    virtual void vf_14();
    virtual void vf_18();
    virtual void vf_1c();
    virtual void vf_20();
    virtual void vf_24();
    virtual void vf_28();
    virtual void vf_2c();
    virtual void vf_30();
    virtual void run();
    virtual void initialize();

    static int s_argc;
    static const char *const *s_argv;

private:
    static RkSystem s_system;
    static RkSystem *s_instance;
    static RkSystem *s_parentInstance; // Actually part of the parent class
    static SceneCreatorStatic *s_sceneCreatorStatic;

    u8 _04[0x54 - 0x04];
    SceneManager *m_sceneManager;
    u8 _58[0x60 - 0x58];
    EGG::Heap *m_relHeap;
    u8 _64[0x74 - 0x64];
};

} // namespace System
