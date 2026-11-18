#pragma once

namespace input {
    enum Button : int {
        ATTACK    = 1 << 0,
        JUMP      = 1 << 1,
        DUCK      = 1 << 2,
        FORWARD   = 1 << 3,
        BACK      = 1 << 4,
        USE       = 1 << 5,
        MOVELEFT  = 1 << 9,
        MOVERIGHT = 1 << 10,
        ATTACK2   = 1 << 11,
        RELOAD    = 1 << 13,
    };

    void Hold(int mask);      // pressed until Release()
    void Release(int mask);
    void Tap(int mask);       // pressed for exactly one tick
    void Apply(void* cmd);    // Applies queued buttons to a numbered command.
}