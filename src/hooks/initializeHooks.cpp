#include "hooks/initializeHooks.hpp"
#include "core/gameMaster.hpp"
#include"log.hpp"
#include "hooks/backstabDetection.hpp"

static BackstabDetect g_BackstabDetectHook;


void InitializeHooks() {
    GameBaseAdress = getGameBaseAdrs();
    LOG("GameBaseAdress: %p", reinterpret_cast<void*>(GameBaseAdress));

    g_BackstabDetectHook.Initialize();
}