#include "core/gameMaster.hpp"
#include "log.hpp"
#include <cstdint>
#include <dlfcn.h>
#include <link.h>

uintptr_t getGameBaseAdrs() {
    // asking Linux if we can have acess to process, nullptr > current process (tf2)
    void* handle = dlopen(nullptr, RTLD_LAZY);
    if (!handle) {
        LOG("Something Broke when Getting Base Adress");
        return 0;
    }

    //dont ask me waht a link map is idfk
    uintptr_t base_address = 0;
    link_map* map = nullptr;
    if (dlinfo(handle, RTLD_DI_LINKMAP, &map) == 0 && map != nullptr) {
        dlclose(handle);
        // map->l_addr is the base load address of the game executable
        base_address = reinterpret_cast<uintptr_t>(map->l_addr);
        LOG("Found game base address: %p", reinterpret_cast<void*>(base_address));
        return base_address;
    }
    else {LOG("Something bad happened i think..");}


    dlclose(handle);
    return 0;
}