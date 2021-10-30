#pragma once

#include <egg/core/eggHeap.hxx>

extern "C" {
    #include <rvl/os/OSThread.h>
}

namespace System {
namespace Rel {

typedef void (*EntryFunction)(void);

void *getStart();

void *getEnd();

u32 getSize();

class Loader {
public:
    Loader(EGG::Heap *heap);

    ~Loader();

    EntryFunction poke();

private:
    static void *start(void *loader);

    EntryFunction run();

    EGG::Heap *m_heap;
    OSThread m_thread;
};

} // namespace Rel
} // namespace System
