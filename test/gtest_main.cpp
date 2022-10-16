#include <chrono>
#include <random>

#include <gtest/gtest.h>

#include <QByteArray>
#include <QString>

const qsizetype default_string_length = 256;

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
    std::mt19937_64 e(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    
    std::uniform_int_distribution<qsizetype> U_UCase('A', 'Z');
    std::uniform_int_distribution<qsizetype> U_LCase('a', 'z');
    std::uniform_int_distribution<qsizetype> U_Len(1, default_string_length);
    {

    }
}