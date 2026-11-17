#include "hooks/initializeHooks.hpp"
#include "core/gameMaster.hpp"
#include"log.hpp"

void InitializeHooks() {
    GameBaseAdress = getGameBaseAdrs();
    LOG("GameBaseAdress: %p", reinterpret_cast<void*>(GameBaseAdress));
}