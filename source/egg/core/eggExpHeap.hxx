#pragma once

#include "eggHeap.hxx"

namespace EGG {

class ExpHeap : public Heap {
public:
    static ExpHeap *create(void *block, u32 size, u16 attrs);
};

} // namespace EGG
