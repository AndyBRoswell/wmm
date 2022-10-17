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
    auto next_int = [](const auto a, const auto b) {
        return tiny_random::number::integer(a, b);
    };
    for (size_t i = 0; i < 10; ++i) {
        const QByteArray s;
    }
}
