#pragma once
#include <cstdint>
#include <../libs/funchook-1.1.3/include/funchook.h>

class hooks {
protected:
    funchook_t* m_funchook = nullptr;
    void* m_original = nullptr;
public:

    hooks() {
        m_funchook = funchook_create();
    }

    virtual ~hooks() {
        if (m_funchook) {
            // Ensure we uninstall before destroying the handle
            funchook_uninstall(m_funchook, 0);
            funchook_destroy(m_funchook);
        }
    }
    
    virtual uintptr_t GetTargetAddress() const = 0;
    virtual const char* GetSignature() const = 0;
    virtual void* GetHookFunction() const = 0;
    virtual void Initialize() = 0;
    virtual void Kill() = 0;

    int CreateHook();

    int DestroyHook();

};
