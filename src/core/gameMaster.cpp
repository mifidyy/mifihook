#include "core/gameMaster.hpp"
#include "log.hpp"
#include <cstdint>
#include <dlfcn.h>
#include <link.h>
#include <cstring>

uintptr_t getGameBaseAdrs() {
    //get da perms to read nullptr (main process (tf2 (ithink)))
    void* handle = dlopen(nullptr, RTLD_LAZY);
    if (!handle) { LOG("dlopen(nullptr) failed"); return 0; }

    //dis is where the loot will be AT
    link_map* map = nullptr;
    uintptr_t result = 0;

    //get the info
    if (dlinfo(handle, RTLD_DI_LINKMAP, &map) == 0 && map) {
        link_map* curr = map;
        //now look thorugh the map i think??
        while (curr->l_prev) curr = curr->l_prev;
        //pretty sure we are still itterating through.. or stmh dont ask
        for (; curr; curr = curr->l_next) {
            if (!curr->l_name || !*curr->l_name) continue;

            //uh yes
            const char* slash = strrchr(curr->l_name, '/');
            const char* file  = slash ? slash + 1 : curr->l_name;

            // exact match on file name, plus the 64-bit TF2 path
            if (strcmp(file, "client.so") == 0 &&
                strstr(curr->l_name, "linux64") != nullptr) {
                result = (uintptr_t)curr->l_addr;
                LOG("client.so: %s  l_addr=%p", curr->l_name, (void*)result);
                break;
            }
        }
    }

    dlclose(handle);
    if (!result) LOG("client.so not found (not loaded yet?)");
    return result;
}