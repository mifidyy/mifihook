#include "hooks/hooks.hpp"
#include "log.hpp"

int hooks::CreateHook(){
    // Create the funchook session handle
    if (!m_funchook) {
        LOG("Failure with reading/acessing funchook variable.");
        return 1;
    }

    // Assign your raw address to the function pointer variable
    m_original = reinterpret_cast<void*>(GetTargetAddress());
    
    // Prepare the hook: 
    // Notice we pass &original_func (a void**) because funchook modifies it 
    // to point to the trampoline function block.
    int rv = funchook_prepare(
        m_funchook, 
        reinterpret_cast<void**>(&m_original), 
        reinterpret_cast<void*>(GetHookFunction())
    );
    
    if (rv != 0) {
        LOG("funchook_prepare failed with error code: %d", rv);
        funchook_destroy(m_funchook);
        return rv;
    }

    // Install the hook: 
    // This patches memory (changes permissions and writes the jump instruction to your hook).
    rv = funchook_install(m_funchook, 0);
    if (rv != 0) {
        LOG("funchook_install failed with error code: %d", rv);
        return rv;
    } else {
        LOG("Function sucessfully hooked!");
    }

    // Clean up the management handle (the active hooks remain installed)
    funchook_destroy(m_funchook);

    return 0;
}
int hooks::DestroyHook(){
    if (!m_funchook) {
        LOG("Failure with reading/acessing funchook variable.");
        return 1;
    }

    // 1. Prepare the exact same hook pairing so funchook knows what to undo
    int rv = funchook_prepare(
        m_funchook, 
        reinterpret_cast<void**>(&m_original), 
        reinterpret_cast<void*>(GetHookFunction())
    );
    
    if (rv != 0) {
        LOG("funchook_prepare for uninstall failed with error code: %d", rv);
        funchook_destroy(m_funchook);
        return rv;
    }

    // 2. Uninstall the hook (patches the memory back to the original instructions)
    rv = funchook_uninstall(m_funchook, 0);
    if (rv != 0) {
        LOG("funchook_uninstall failed with error code: %d", rv);
        return rv;
    } else {
        LOG("Hook successfully removed! Original function restored.");
    }

    // 3. Clean up the management handle
    funchook_destroy(m_funchook);

    return 0;
}