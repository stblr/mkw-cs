#pragma once

#include "eggDisposer.hxx"

extern "C" {
    #include <stddef.h>
}

namespace EGG {

class Allocator;

class Heap : public Disposer {
public:
    static void initialize();

    virtual ~Heap();

    virtual s32 getHeapKind() const = 0;

    virtual void initAllocator(Allocator *allocator, s32 align) = 0;

    virtual void *alloc(u32 size, s32 align) = 0;

    virtual void free(void *block) = 0;

    virtual void destroy() = 0;

    virtual u32 resizeForMBlock(void *block, u32 size) = 0;

    virtual u32 getAllocatableSize(s32 align) = 0;

    virtual u32 adjust() = 0;

    Heap *becomeCurrentHeap();

private:
    u8 _10[0x38 - 0x10];
};

} // namespace EGG

void *operator new(size_t size);

void *operator new(size_t size, EGG::Heap *heap, int align);
