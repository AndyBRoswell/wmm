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

// Tests of WMM begin here.
TEST(Algorithm, StringIeq) { // ieq is from powershell
    auto next_int = [](const auto a, const auto b) -> auto {
        return tiny_random::number::integer(a, b);
    };
    auto next_str = [](const size_t l, const tiny_random::chr::ASCII_char_type t = tiny_random::chr::ASCII_char_type::printable) {
        return tiny_random::chr::ASCII_string(l, t);
    };
    for (size_t i = 0; i < 10; ++i) {
        const QByteArray s[2] = { "", s[0].toLower() };
    }
}
