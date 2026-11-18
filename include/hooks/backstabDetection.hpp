#pragma once

#include "hooks.hpp"
#include <cstdint>
#include <cstdio>

class BackstabDetect : public hooks {
public:
    BackstabDetect() = default;
    ~BackstabDetect() override = default;

    using BackstabDetectFn = bool (*)(void* knife, void* target);

    inline static BackstabDetectFn s_original = nullptr;

    //ignore this signature. this is old, wrong signature just ignore.
    static constexpr const char* kSignature = "48 85 f6 74 4b 55 31 c9 48 89 f2 41 b8 01 00 00 00 48 89 e5 41 55 49 89 fd 31 ff 41 54 49 89 f4 48 8d 35 ?? ?? ?? ?? e8 ?? ?? ?? ?? 85 c0 75 10 4c 89 e6 4c 89 ef 41 5c 41 5d 5d e9 ?? ?? ?? ??";

    uintptr_t GetTargetAddress() const override {return target_address; };
    
    const char* GetSignature() const override { return kSignature; }

    void* GetHookFunction() const override { return reinterpret_cast<void*>(&Hooked); }

    void Initialize() override;
    void Kill() override;

private:

    uintptr_t target_address = 0;

    // The actual detour function (must be static so it has a standard C function pointer signature)
    static bool Hooked(void* knife, void* target);

};