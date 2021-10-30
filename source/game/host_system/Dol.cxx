#include "Dol.hxx"

extern u8 dolStart;
extern u8 dolEnd;

namespace System {
namespace Dol {

void *getStart() {
    return &dolStart;
}

void *getEnd() {
    return &dolEnd;
}

u32 getSize() {
    return &dolEnd - &dolStart;
}

} // namespace Dol
} // namespace System
