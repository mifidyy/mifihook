#include "core/input.hpp"

#include <atomic>
#include <cstddef>
#include <cstring>

namespace {
    // CUserCmd field offsets in the 64-bit Linux client.
    constexpr std::size_t kCommandNumberOffset = 0x08;
    constexpr std::size_t kButtonsOffset = 0x28;

    std::atomic<int> g_held{0};
    std::atomic<int> g_tap{0};
}

void input::Hold(int mask) {
    g_held.fetch_or(mask);
}

void input::Release(int mask) {
    g_held.fetch_and(~mask);
}

void input::Tap(int mask) {
    g_tap.fetch_or(mask);
}

void input::Apply(void* cmd) {
    if (!cmd) {
        return;
    }

    auto* bytes = static_cast<char*>(cmd);
    int commandNumber = 0;
    std::memcpy(&commandNumber, bytes + kCommandNumberOffset,
                sizeof(commandNumber));

    // ExtraMouseSample passes a dummy command with number zero.
    if (commandNumber == 0) {
        return;
    }

    const int pendingButtons = g_held.load() | g_tap.exchange(0);
    if (pendingButtons == 0) {
        return;
    }

    int buttons = 0;
    std::memcpy(&buttons, bytes + kButtonsOffset, sizeof(buttons));
    buttons |= pendingButtons;
    std::memcpy(bytes + kButtonsOffset, &buttons, sizeof(buttons));
}
