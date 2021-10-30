#pragma once

#include <Common.hxx>

namespace EGG {

class Disposer {
protected:
    Disposer();

    virtual ~Disposer();

private:
    u8 _4[0x10 - 0x4];
};

} // namespace EGG
