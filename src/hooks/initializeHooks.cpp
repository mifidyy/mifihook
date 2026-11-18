#include "hooks/initializeHooks.hpp"
#include "core/gameMaster.hpp"
#include"log.hpp"
#include "hooks/backstabDetection.hpp"
#include "hooks/createMove.hpp"
#include "features/settings.hpp"

static BackstabDetect g_BackstabDetectHook;

static CreateMove g_CreateMoveHook;


void InitializeHooks() {
    LOG("Initializing hooks...");
    GameBaseAdress = getGameBaseAdrs();
    LOG("GameBaseAdress: %p", reinterpret_cast<void*>(GameBaseAdress));

    g_BackstabDetectHook.Initialize();
    g_CreateMoveHook.Initialize();
    autostab(true);
}

void DestroyHooks() {
    LOG("Destroying hooks...");

    g_BackstabDetectHook.DestroyHook();
    g_CreateMoveHook.DestroyHook();
}