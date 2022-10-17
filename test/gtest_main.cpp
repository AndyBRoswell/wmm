#include <chrono>
#include <random>

#include <gtest/gtest.h>

#include <QByteArray>
#include <QString>

#include "tiny_random.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
    EXPECT_EQ(7 * 6, 43);
}

TEST(Algorithm, Random) {
    std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN + 1, INTMAX_MAX);
    constexpr int a = 1, b = 256;
    constexpr auto L = b - a + 1;
    for (size_t i = 0; i < 1e9; ++i) {
        const auto x = max_uniform_int_dist(random_engine);
        const auto r = a + (x % L - (INTMAX_MIN + 1) % L) % L;
        EXPECT_GE(r, a);
        EXPECT_LE(r, b);
    }
}

// Tests of WMM begin here.
TEST(Algorithm, StringIeq) { // ieq is from powershell
    auto next_int = [](const auto a, const auto b) -> auto {
        return tiny_random::number::integer(a, b);
    };
    auto next_str = [](const size_t l, const tiny_random::chr::ASCII_char_type t = tiny_random::chr::ASCII_char_type::printable) {
        return tiny_random::chr::ASCII_string(l, t);
    };
    for (size_t i = 0; i < 10; ++i) {
        const QByteArray s = QByteArray::fromStdString(next_str(next_int(1, 256)));
    }
}
