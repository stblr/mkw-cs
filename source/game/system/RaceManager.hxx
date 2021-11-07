#pragma once

namespace System {

class RaceManager {
public:
    static RaceManager *createInstance();

    void init();
};

} // namespace System
