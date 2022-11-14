#include "util.h"

// std
#include <iostream>

// Qt
#include <QTest>

static void blank_test_message_handler(const QtMsgType type, const QMessageLogContext& context, const QString& msg) {};

void util::disable_test_info() noexcept {
    qInstallMessageHandler(blank_test_message_handler); // ignore warnings
}

void util::enable_test_info() noexcept {
    qInstallMessageHandler(nullptr);
}

std::chrono::high_resolution_clock::duration::rep util::get_tick_count() noexcept { 
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

void util::no_sync_with_stdio() noexcept {
    if (std::ios_base::sync_with_stdio() == true) { std::ios_base::sync_with_stdio(false); }
    if (std::cin.tie() != nullptr) { std::cin.tie(nullptr); }
    if (std::cout.tie() != nullptr) { std::cout.tie(nullptr); }
}
