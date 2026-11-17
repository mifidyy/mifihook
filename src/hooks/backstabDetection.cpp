#include "hooks/backstabDetection.hpp"
#include "features/autostab.hpp"
#include "log.hpp"
#include <link.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <../libs/funchook-1.1.3/include/funchook.h>
#include "log.hpp"

funchook_t *funchook = funchook_create();

bool BackstabDetect::Hooked(void* knife, void* target) {
    bool result = g_original(knife, target);
    
    if (g_log) {
        fprintf(g_log, "knife=%p target=%p -> %d\n", knife, target, (int)result);
        fflush(g_log);
    }
    
    return result;
}

void BackstabDetect::Initialize() {
    LOG("Backstab Detect is being Initialized...");

    LOG("BackstabDetect hook installed");
}

void BackstabDetect::Kill() {
    LOG("Backstab Detect is being removed...");

    LOG("BackstabDetect hook removed");
}

