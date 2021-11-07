#pragma once

namespace Ai {

class AiManager {
public:
    static AiManager *createInstance();

    void init();
};

} // namespace Ai
