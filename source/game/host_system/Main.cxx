#include "Patcher.hxx"
#include "Rel.hxx"

#include <egg/core/eggExpHeap.hxx>

extern "C" {
    #include <rvl/os.h>
}

REPLACE int main(int argc, char **argv) {
    EGG::Heap::initialize();
    void *start = OSGetMEM1ArenaLo();
    u32 size = static_cast<u8 *>(OSGetMEM1ArenaHi()) - static_cast<u8 *>(OSGetMEM1ArenaLo());
    EGG::Heap *heap = EGG::ExpHeap::create(start, size, 0);
    heap->becomeCurrentHeap();

    System::Rel::load(heap);
    System::Patcher::patch(System::Patcher::Binary::Rel);

    return 0;
}
