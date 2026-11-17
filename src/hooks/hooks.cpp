#include "hooks/hooks.hpp"
#include <../libs/funchook-1.1.3/include/funchook.h>
#include "log.hpp"

int hooks::CreateHook(){
    LOG("CREATEHOOK IS RUNNING");
    // Create the funchook session handle
    if (!m_funchook) {
        LOG("Failure with reading/acessing funchook variable; creating new one");
        m_funchook = funchook_create();
    }

    // Assign your raw address to the function pointer variable
    LOG("ASSIGNING ADRESS TO VARIABLE");
    m_original = reinterpret_cast<void*>(GetTargetAddress());
    
    // Prepare the hook: 
    // Notice we pass &original_func (a void**) because funchook modifies it 
    // to point to the trampoline function block.
    LOG("PREPPING HOOK");
    int rv = funchook_prepare(
        m_funchook, 
        reinterpret_cast<void**>(&m_original), 
        reinterpret_cast<void*>(GetHookFunction())
    );
    
    LOG("DONE PREPPING");
    if (rv != 0) {
        LOG("funchook_prepare failed with error code: %d", rv);
        funchook_destroy(m_funchook);
        return rv;
    }

    // Install the hook: 
    // This patches memory (changes permissions and writes the jump instruction to your hook).
    LOG("INSTALLING HOOK");
    rv = funchook_install(m_funchook, 0);
    if (rv != 0) {
        LOG("funchook_install failed with error code: %d", rv);
        return rv;
    } else {
        LOG("Function sucessfully hooked!");
    }

    return 0;
}
int hooks::DestroyHook(){
    if (!m_funchook) {
        LOG("Failure with reading/acessing funchook variable");
        return 1;
    }

    // 1. Uninstall the hook (patches the memory back to the original instructions)
    int rv = funchook_uninstall(m_funchook, 0);
    if (rv != 0) {
        LOG("funchook_uninstall failed with error code: %d", rv);
        return rv;
    } else {
        LOG("Hook successfully removed! Original function restored.");
    }

    // 2. Clean up the management handle
    funchook_destroy(m_funchook);

    m_funchook = nullptr;

    return 0;
}