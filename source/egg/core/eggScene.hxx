#pragma once

#include "eggDisposer.hxx"
#include "eggHeap.hxx"

namespace EGG {

class Scene : public Disposer {
public:
    Scene();

    virtual ~Scene();

    virtual void calc();

    virtual void draw();

    virtual void enter();

    virtual void exit();

    virtual void reinit();

    virtual void incoming_childDestroy();

    virtual void outgoing_childCreate();

private:
    u8 _10[0x14 - 0x10];

protected:
    Heap *m_heapMem1;

private:
    u8 _18[0x30 - 0x18];
};

} // namespace EGG
