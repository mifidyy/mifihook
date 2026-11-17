#include "hooks/initializeHooks.hpp"
#include "core/gameMaster.hpp"
#include"log.hpp"
#include "hooks/backstabDetection.hpp"

static BackstabDetect g_BackstabDetectHook;


void InitializeHooks() {
    LOG("Initializing hooks...");
    GameBaseAdress = getGameBaseAdrs();
    LOG("GameBaseAdress: %p", reinterpret_cast<void*>(GameBaseAdress));

    g_BackstabDetectHook.Initialize();
}

void DestroyHooks() {
    LOG("Destroying hooks...");

    g_BackstabDetectHook.DestroyHook();
}