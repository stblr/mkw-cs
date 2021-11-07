#include "KartUnit.hxx"

namespace Kart {

REPLACE void KartUnit::init() {
    createTires();
    createSub();
    createGraphics();
    //initTires();
    // FIXME HACK to have the correct vtable offset
    void *hack0 = this;
    void (*hack1)(void *) = (void (*)(void *))(((u32 **)hack0)[3][5]);
    hack1(hack0);
    initTirePhysics();
    m_accessor.params = m_params;
}

} // namespace Kart
