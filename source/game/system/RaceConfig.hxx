#pragma once

#include <Common.hxx>

namespace System {

class RaceConfig {
public:
    enum class PlayerType : u32 {
        Local = 0x0,
        Cpu = 0x1,
        Ghost = 0x3,
        Remote = 0x4,
        None = 0x5,
    };

    struct Player {
        u8 _00[0x08];
        u32 vehicleId;
        u32 characterId;
        PlayerType playerType;
        u8 _14[0xf0 - 0x14];
    };

    enum class EngineClass : u32 {
        Cc50 = 0x0,
        Cc100 = 0x1,
        Cc150 = 0x2,
        CcBattle = 0x3,
    };

    enum class GameMode : u32 {
        GrandPrix = 0x0,
        OfflineVs = 0x1,
        TimeAttack = 0x2,
        OfflineBattle = 0x3,
        MissionRun = 0x4,
        GhostBattle = 0x5,
        PrivateVs = 0x7,
        PublicVs = 0x8,
        PublicBattle = 0x9,
        PrivateBattle = 0xa,
        Awards = 0xb,
        Credits = 0xc,
    };

    enum class BattleType : u32 {
        Balloon = 0x0,
        Coin = 0x1,
    };

    enum class CpuMode : u32 {
        Easy = 0x0,
        Medium = 0x1,
        Hard = 0x2,
        None = 0x3,
    };

    enum class ItemMode : u32 {
        Balanced = 0x0,
        Frantic = 0x1,
        Strategic = 0x2,
        None = 0x3,
    };

    struct Scenario {
        u8 _000[0x008];
        Player players[12];
        u32 courseId;
        EngineClass engineClass;
        GameMode gameMode;
        u32 gameType;
        BattleType battleType;
        CpuMode cpuMode;
        ItemMode itemMode;
        s8 localPlayerIds[4];
        u32 cupId;
        u8 raceNumber;
        u8 lapCount;
        u8 _b6e[0xb70 - 0xb6e];
        u32 modeFlags;
        u8 _b74[0xbf0 - 0xb74];
    };

    static RaceConfig *createInstance();

    void initRace();
    Scenario &getMenuScenario() {
        return m_menuScenario;
    }

private:
    u8 _0000[0x0020 - 0x0000];
    Scenario m_raceScenario;
    Scenario m_menuScenario;
    Scenario m_awardsScenario;
    u8 _23d4[0x73f0 - 0x23d4];
};

} // namespace System
