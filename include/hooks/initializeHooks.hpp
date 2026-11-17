#pragma once
#include <cstdint>
//I suck at naming, this also gets game address + destroys hooks.
inline uintptr_t GameBaseAdress;
void InitializeHooks();
void DestroyHooks();