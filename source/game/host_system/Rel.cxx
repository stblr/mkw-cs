#include "Rel.hxx"

extern "C" {
    #include <rvl/dvd.h>
    #include <rvl/os.h>
    #include <rvl/os/OSCache.h>
    #include <rvl/os/OSModule.h>

    #include <string.h>
}

extern u8 relStart;
extern u8 relEnd;

namespace System {
namespace Rel {

void *getStart() {
    return &relStart;
}

void *getEnd() {
    return &relEnd;
}

u32 getSize() {
    return &relEnd - &relStart;
}

Loader::Loader(EGG::Heap *heap) : m_heap(heap) {
    u32 stackSize = 0x5000;
    u8 *stack = (u8 *)m_heap->alloc(stackSize, 0x20);
    void *stackBase = stack + stackSize;
    OSCreateThread(&m_thread, start, this, stackBase, stackSize, 20, 0);
    OSResumeThread(&m_thread);
}

Loader::~Loader() {
    OSDetachThread(&m_thread);
}

EntryFunction Loader::poke() {
    void *entry;
    if (!OSJoinThread(&m_thread, &entry)) {
        return nullptr;
    }

    return reinterpret_cast<EntryFunction>(entry);
}

void *Loader::start(void *loader) {
    EntryFunction entry = static_cast<Loader *>(loader)->run();
    return reinterpret_cast<void *>(entry);
}

EntryFunction Loader::run() {
    DVDFileInfo fileInfo;
    if (!DVDOpen("/rel/StaticR.rel", &fileInfo)) {
        return nullptr;
    }

    s32 size = OSRoundUp32B(fileInfo.length);
    void *src = m_heap->alloc(size, 0x20);
    if (!src) {
        DVDClose(&fileInfo);
        return nullptr;
    }

    s32 result = DVDRead(&fileInfo, src, size, 0);
    DVDClose(&fileInfo);
    if (result != size) {
        return nullptr;
    }

    void *dst = getStart();
    auto *srcHeader = static_cast<OSModuleHeader *>(src);
    memcpy(dst, src, srcHeader->fixSize);
    ICInvalidateRange(dst, srcHeader->fixSize);
    auto *dstHeader = static_cast<OSModuleHeader *>(dst);

    void *bss = static_cast<u8 *>(dst) + OSRoundUp32B(srcHeader->fixSize);

    dstHeader->info.sectionInfoOffset += reinterpret_cast<u32>(dst);
    auto *dstSectionInfo = reinterpret_cast<OSSectionInfo *>(dstHeader->info.sectionInfoOffset);
    for (u32 i = 1; i < dstHeader->info.numSections; i++) {
        if (dstSectionInfo[i].offset != 0) {
            dstSectionInfo[i].offset += reinterpret_cast<u32>(dst);
        } else if (dstSectionInfo[i].size != 0) {
            dstSectionInfo[i].offset = reinterpret_cast<u32>(bss);
        }
    }

    dstHeader->impOffset += reinterpret_cast<u32>(src);
    auto *importInfo = reinterpret_cast<OSImportInfo *>(dstHeader->impOffset);
    for (u32 i = 0; i < dstHeader->impSize / sizeof(OSImportInfo); i++) {
        importInfo[i].offset += reinterpret_cast<u32>(src);
    }

    Relocate(nullptr, dstHeader);
    Relocate(dstHeader, dstHeader);

    m_heap->free(src);

    auto *prologSectionInfo = dstSectionInfo + dstHeader->prologSection;
    return reinterpret_cast<EntryFunction>(prologSectionInfo->offset + dstHeader->prolog);
}

} // namespace Rel
} // namespace System
