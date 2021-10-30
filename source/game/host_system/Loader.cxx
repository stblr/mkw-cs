#include "Rel.hxx"

extern "C" {
    #include <rvl/dvd.h>
    #include <rvl/os.h>
    #include <rvl/os/OSCache.h>
}

namespace System {
namespace Loader {

typedef void (*PayloadEntryFunction)();

PayloadEntryFunction loadPayload() {
    DVDFileInfo fileInfo;
    if (!DVDOpen("/cs/payload.bin", &fileInfo)) {
        return nullptr;
    }

    s32 size = OSRoundUp32B(fileInfo.length);
    void *payload = OSAllocFromMEM1ArenaLo(size, 0x20);
    s32 result = DVDRead(&fileInfo, payload, size, 0);
    DVDClose(&fileInfo);
    if (result != size) {
        return nullptr;
    }

    ICInvalidateRange(payload, size);

    return reinterpret_cast<PayloadEntryFunction>(payload);
}

void main() {
    OSInit();

    OSAllocFromMEM1ArenaLo(Rel::getSize(), 0x20);

    PayloadEntryFunction entry = loadPayload();
    if (!entry) {
        GXColor fg = { 255, 255, 255, 255 };
        GXColor bg = { 0, 0, 0, 255 };
        OSFatal(fg, bg, "Couldn't load mkw-cs payload!");
    }

    entry();
}

} // namespace Loader
} // namespace System
