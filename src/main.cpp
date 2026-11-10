#include <atomic>
#include <thread>
#include <chrono>

static std::atomic<bool> running{false};
static std::thread worker;

extern "C" void mod_init() {
    running = true;
    worker = std::thread([]{
        while (running) {
            // work
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
}

extern "C" void mod_shutdown() {
    running = false;
    worker.join();
}