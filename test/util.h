#include <chrono>

class util {
public:
    static void disable_test_info() noexcept;
    static void enable_test_info() noexcept;

    static std::chrono::high_resolution_clock::duration::rep get_tick_count() noexcept;

    static void no_sync_with_stdio() noexcept;
};
