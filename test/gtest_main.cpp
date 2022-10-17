#include <chrono>
#include <random>

#include <QByteArray>
#include <QString>

#include "tiny_random.h"

#include <gtest/gtest.h>

// Modules tested

#include "../src/Algorithm.h"

// Demonstrate some basic assertions.
//TEST(HelloTest, BasicAssertions) {
//    // Expect two strings not to be equal.
//    EXPECT_STRNE("hello", "world");
//    // Expect equality.
//    EXPECT_EQ(7 * 6, 42);
//    EXPECT_EQ(7 * 6, 43);
//}
//

//TEST(TestAlgorithm, Mod) {
//    std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
//    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INT_MIN, INT_MAX);
//    //std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
//    const auto m = INT_MIN;
//    //const auto m = INTMAX_MIN;
//    //const auto a = 1, b = 256;
//    const auto a = 0x20, b = 0x7E;
//    const auto L = b - a + 1;
//    for (size_t i = 0; i < 1e9; ++i) {
//        const intmax_t x = max_uniform_int_dist(random_engine);
//        const intmax_t A = (x - m) % L;
//        const intmax_t B = (x % L - m % L) % L;
//        EXPECT_EQ(A, B);
//    }
//}

//TEST(TestAlgorithm, Random) {
//    auto mod = [](const auto& N, const auto& D) constexpr {
//        const auto r = N % D;
//        if (D > 0) { return r >= 0 ? r : r + D; }
//        else { return r <= 0 ? r : r + D; }
//    };
//    std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
//    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
//    constexpr intmax_t B[][2] = { { 1, 256 }, { 0x20, 0x7E }, { 0, 10 }, { -15, 13 }, { -997, -122 }, { 0, 0 }, { -1e6, 1e6 }, { -1.5e10, 1.6e10 } };
//    for (size_t h = 0; h < sizeof(B) / (2 * sizeof(intmax_t)); ++h) {
//        const auto a = B[h][0], b = B[h][1];
//        const auto L = b - a + 1;
//        for (size_t i = 0; i < 1 * 1e9; ++i) {
//            const auto x = max_uniform_int_dist(random_engine);
//            const auto r = a + mod(mod(x, L) - mod(INTMAX_MIN, L), static_cast<intmax_t>(L));
//            EXPECT_GE(r, a);
//            EXPECT_LE(r, b);
//        }
//    }
//}

// Tests of WMM begin here.
TEST(Algorithm, StringIeq) { // ieq is from powershell
    namespace wmm = WritingMaterialsManager;
    
    auto next_int = [](const auto a, const auto b) -> auto {
        return tiny_random::number::integer(a, b);
    };
    auto next_str = [](const size_t l, const tiny_random::chr::ASCII_char_type t = tiny_random::chr::ASCII_char_type::printable) {
        return tiny_random::chr::ASCII_string(l, t);
    };

    constexpr size_t g = 10;        // group count of test data
    constexpr size_t lmax = 256;    // max length of test strings

    // csae-insensitive hasher
    constexpr wmm::CaseInsensitiveHasher hasher;
    for (size_t i = 0; i < g; ++i) {
        const QByteArray s = QByteArray::fromStdString(next_str(next_int(1ull, lmax))), t = s.toLower(); // 1ull -> 1uz since C++23
        const QString u = QString::fromStdString(next_str(next_int(1ull, lmax))), v = s.toLower();
        const size_t h[2][2] = { { hasher(s), hasher(t) }, { hasher(u), hasher(v) }};
        EXPECT_EQ(h[0][0], h[0][1]);
        EXPECT_EQ(h[1][0], h[1][1]);
    }

    // case-insensitive comparator
    constexpr wmm::CaseInsensitiveStringComparator comparator;
    for (size_t i = 0; i < g; ++i) {
        const QString s = QString::fromStdString(next_str(next_int(1ull, lmax))), t = s.toLower();
    }
}
