#pragma once

namespace Race {

class RaceGlobals {
public:
    static void init();
    static u8 kartCount() {
        return s_globals.m_kartCount;
    }
    static bool isTimeAttack() {
        return s_globals.m_isTimeAttack;
    }

private:
    static RaceGlobals s_globals;

    u8 m_kartCount;
    u8 _01[0x04 - 0x01];
    bool m_isTimeAttack;
    u8 _05[0x20 - 0x05];
};

} // namespace Race
