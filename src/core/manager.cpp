#include "core/manager.hpp"
#include <chrono>
#include <iostream>

CoreManager& CoreManager::GetInstance() {
    static CoreManager instance;
    return instance;
}

CoreManager::~CoreManager() {
    Shutdown();
}

void CoreManager::Initialize() {
    // std::atomic::exchange makes this call thread-safe and idempotent
    if (m_running.exchange(true)) {
        return; // Already initialized, do nothing
    }

    std::cout << "[CoreManager] Starting background worker...\n";
    
    // Spawn the background thread running our BackgroundLoop method
    m_worker = std::thread(&CoreManager::BackgroundLoop, this);
}

void CoreManager::Shutdown() {
    if (!m_running.exchange(false)) {
        return; // Already stopped, do nothing
    }

    std::cout << "[CoreManager] Stopping background worker...\n";

    // Wait for the thread loop to finish its current iteration and exit
    if (m_worker.joinable()) {
        m_worker.join();
    }
}

void CoreManager::BackgroundLoop() {
    while (m_running) {
        // =========================================================
        // INSERT WHAT YOU WANT TO RUN IN THE BACKGROUND HERE:
        // Examples:
        // 1. Auto-save config settings to disk every 5 seconds
        // 2. Refresh cached player lists / memory pointers
        // 3. Process async network requests or IPC messages
        // =========================================================

        std::cout << "[Background Loop] Heartbeat check...\n";

        // Sleep to avoid burning 100% CPU on a single core
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
}