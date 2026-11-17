#include "../include/core/manager.hpp"
#include "hooks/initializeHooks.hpp"
#include "log.hpp"

extern "C" void mod_init() {
    LOG("Cheat is initializing.");
    CoreManager::GetInstance().Initialize();
    InitializeHooks();
}

extern "C" void mod_shutdown() {
    CoreManager::GetInstance().Shutdown();
}

__attribute__((constructor)) void load_plugin() {
    LOG("Cheat has been loaded.");

}