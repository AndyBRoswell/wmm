#include <gtest/gtest.h>

#include <QByteArray>
#include <QString>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

// Tests of WMM begin here.
TEST(Algorithm, StringIeq) { // ieq is from powershell

}