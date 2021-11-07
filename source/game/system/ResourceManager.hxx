#pragma once

#include <egg/core/eggHeap.hxx>

namespace System {

class MultiDvdArchive;

enum class ResourceType : u32 {
    RaceCommon = 0,
};

class ResourceManager {
public:
    static ResourceManager *createInstance();
    static ResourceManager *getInstance() {
        return s_instance;
    }

    MultiDvdArchive *load(ResourceType type, EGG::Heap *heap, const char *name);
    MultiDvdArchive *loadCourse(u32 courseId, EGG::Heap *heap, bool splitScreen);
    void process();

private:
    static ResourceManager *s_instance;

    u8 _000[0x61c];
};

} // namespace System
