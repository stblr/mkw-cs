#pragma once

#include <Common.hxx>

namespace System {

class CourseMap {
public:
    static CourseMap *createInstance();
    static CourseMap *getInstance() {
        return s_instance;
    }

    void init();

private:
    static CourseMap *s_instance;

    u8 _00[0x54];
};

} // namespace System
