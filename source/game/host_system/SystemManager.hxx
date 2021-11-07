#pragma once

#include <egg/core/eggHeap.hxx>

extern "C" {
    #include <rvl/os/OSMessage.h>
}

namespace System {

class SystemManager {
public:
    static SystemManager *createInstance(EGG::Heap *heap);
    static SystemManager *getInstance() {
        return s_instance;
    }

    OSMessageQueue *getMessageQueue() {
        return &m_messageQueue;
    }

private:
    static SystemManager *s_instance;

    u8 _0000[0x00b8];
    OSMessageQueue m_messageQueue;
    u8 _00d8[0x1100 - 0x00d8];
};

} // namespace System
