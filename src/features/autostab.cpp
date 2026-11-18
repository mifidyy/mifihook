#include "features/autostab.hpp"
#include "features/settings.hpp"
#include "core/input.hpp"
#include "log.hpp"

void autostab(bool on){
    if(on){
        autostabEnabled = 1;
    }
    else {
        autostabEnabled = 0;
    }
}

void trigger(){
    if(autostabEnabled) {
        LOG("Autostabbing");
        input::Tap(input::ATTACK);
    }
}