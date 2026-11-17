//sorry to break this to u this is all claude idk how ts works
#pragma once

#include <thread>
#include <atomic>

class CoreManager {
public:
    // Returns the address of Core Manager
    static CoreManager& GetInstance();

    // Lifecycle controls called from mod_init / mod_shutdown
    void Initialize();
    void Shutdown();

    // Check if the background loop is running, const defines the function as only reading
    bool IsRunning() const { return m_running; }

private:
    CoreManager() = default;
    ~CoreManager();

    // Prevent copy/move construction to preserve Singleton guarantee
    CoreManager(const CoreManager&) = delete;
    CoreManager& operator=(const CoreManager&) = delete;

    // The function that runs inside the worker thread
    void BackgroundLoop();

    std::atomic<bool> m_running{false};
    std::thread m_worker;
};