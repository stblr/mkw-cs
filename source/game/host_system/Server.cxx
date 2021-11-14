#include "Server.hxx"

#include "Patcher.hxx"
#include "Rel.hxx"
#include "RkSystem.hxx"
#include "SystemManager.hxx"

#include "../ai/AiManager.hxx"
#include "../item/ItemManager.hxx"
#include "../kart/KartManager.hxx"
#include "../object/ObjectManager.hxx"
#include "../race/BoxColManager.hxx"
#include "../race/RaceGlobals.hxx"
#include "../race/RenderGroupManager.hxx"
#include "../system/CourseMap.hxx"
#include "../system/GameScene.hxx"
#include "../system/InputManager.hxx"
#include "../system/RaceConfig.hxx"
#include "../system/RaceManager.hxx"
#include "../system/ResourceManager.hxx"
#include "../system/RootScene.hxx"

#include <egg/core/eggDvdFile.hxx>
#include <egg/core/eggExpHeap.hxx>

extern "C" {
    #include <rvl/os.h>
}

namespace System {
namespace Server {

void run() {
    EGG::Heap::initialize();
    void *start = OSGetMEM1ArenaLo();
    u32 size = static_cast<u8 *>(OSGetMEM1ArenaHi()) - static_cast<u8 *>(OSGetMEM1ArenaLo());
    auto *heap = EGG::ExpHeap::create(start, size, 0);
    heap->becomeCurrentHeap();

    Rel::load(heap);
    Patcher::patch(Patcher::Binary::Rel);

    EGG::DvdFile::initialize();
    SystemManager::createInstance(heap);

    RootScene::createStubInstance(heap);
    GameScene::createStubInstance(heap);

    auto *resourceManager = ResourceManager::createInstance();
    InputManager::createInstance();
    auto *raceConfig = RaceConfig::createInstance();

    auto &menuScenario = raceConfig->getMenuScenario();
    menuScenario.players[0].playerType = RaceConfig::PlayerType::Ghost;
    for (u32 i = 1; i < 12; i++) {
        menuScenario.players[i].playerType = RaceConfig::PlayerType::None;
    }
    menuScenario.courseId = 0x18;
    menuScenario.engineClass = RaceConfig::EngineClass::Cc150;
    menuScenario.gameMode = RaceConfig::GameMode::TimeAttack;
    menuScenario.gameType = 0;
    menuScenario.cpuMode = RaceConfig::CpuMode::Medium;
    menuScenario.itemMode = RaceConfig::ItemMode::Balanced;
    menuScenario.lapCount = 3;
    menuScenario.modeFlags = 0;

    raceConfig->initRace();
    resourceManager->load(ResourceType::RaceCommon, heap, nullptr);
    resourceManager->loadCourse(0x18 /* rMC3 */, heap, false);

    auto *courseMap = CourseMap::createInstance();
    courseMap->init();
    auto *raceManager = RaceManager::createInstance();
    Race::RenderGroupManager::createInstance();
    Race::BoxColManager::createInstance();
    Race::RaceGlobals::init();
    auto *aiManager = Ai::AiManager::createInstance();
    auto *kartManager = Kart::KartManager::createInstance();
    auto *itemManager = Item::ItemManager::createInstance();
    auto *objectManager = Object::ObjectManager::createInstance();

    kartManager->init();
    raceManager->init();
    itemManager->init();
    objectManager->init();
    aiManager->init();
}

} // namespace Server
} // namespace System
