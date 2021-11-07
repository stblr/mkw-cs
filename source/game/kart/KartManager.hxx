#pragma once

namespace Kart {

class KartManager {
public:
    static KartManager *createInstance();

    void init();
};

} // namespace Kart
