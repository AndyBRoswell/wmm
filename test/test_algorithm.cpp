// std
#include <chrono>
#include <random>

// Qt
#include <QByteArray>

// googletest
#include <gtest/gtest.h>

// this software
#include "tiny_random.h"

// Tests for tests begins here
TEST(TestAlgorithm, Rem) {
    GTEST_SKIP();
    std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INT_MIN, INT_MAX);
    constexpr auto m = INT_MIN;
    constexpr auto a = 0x20, b = 0x7E;
    constexpr auto L = b - a + 1;
    for (size_t i = 0; i < 1e3; ++i) {
        const intmax_t x = max_uniform_int_dist(random_engine);
        const intmax_t A = (x - m) % L;
        const intmax_t B = (x % L - m % L) % L;
        EXPECT_EQ(A, B);
    }
}

TEST(TestAlgorithm, Random) {
    auto mod = [](const auto& N, const auto& D) constexpr {
        const auto r = N % D;
        if (D > 0) { return r >= 0 ? r : r + D; }
        else { return r <= 0 ? r : r + D; }
    };
    std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
    constexpr intmax_t B[][2] = { { 1, 256 }, { 0x20, 0x7E }, { 0, 10 }, { -15, 13 }, { -997, -122 }, { 0, 0 }, { -1e6, 1e6 }, { -1.5e10, 1.6e10 }, { 0, 10 + 26 + 26 - 1 } };
    for (size_t h = 0; h < sizeof(B) / (2 * sizeof(intmax_t)); ++h) {
        const auto a = B[h][0], b = B[h][1];
        const auto L = b - a + 1;
        for (size_t i = 0; i < 1 * 1e6; ++i) {
            const auto x = max_uniform_int_dist(random_engine);
            const auto r = a + mod(mod(x, L) - mod(INTMAX_MIN, L), static_cast<intmax_t>(L));
            EXPECT_GE(r, a);
            EXPECT_LE(r, b);
        }
    }
}

TEST(TestAlgorithm, StringConversion) {
    GTEST_SKIP();
    auto next_int = [](const auto a, const auto b) -> auto {
        return tiny_random::number::integer(a, b);
    };
    auto next_str = [](const size_t l, const tiny_random::chr::ASCII_char_type t = tiny_random::chr::ASCII_char_type::alnum) {
        return tiny_random::chr::ASCII_string(l, t);
    };

    constexpr size_t lmax = 256;    // max length of test strings
    for (size_t i = 0; i < 1e5; ++i) {
        const QByteArray s = QByteArray::fromStdString(next_str(next_int(1ull, lmax)));
        //const QByteArray s = "deuhf8uehfiueh8t98tu3489tue98jeoifgoidjgieyt894utioemfodgoieti84eutsfiuofsoijfsojfosjfosfsdi0";
        std::cout << s.toStdString() << std::endl;
    }
}

TEST(TestAlgorithm, Exp) {
    constexpr size_t nsp = 1e6;
    constexpr size_t B[][2] = { { 1, 8 }, { 1, 16 }, { 1, 32 }, { 1, 64 }, { 1, 128 } , { 1, 256 } };
    std::exponential_distribution<double> E(2);
    std::cout << "P(x) = " << E.lambda() << "e^(-" << E.lambda() << "x), x >= 0" << std::endl;
    std::cout << "if (x > M) x = M;\n" << std::endl;
    for (size_t i = 0; i < sizeof(B) / (2 * sizeof(size_t)); ++i) {
        const size_t m = B[i][0], M = B[i][1];
        std::vector<size_t> c(M + 1, 0);
        std::cout << "[" << m << ", " << M << "]" << std::endl;
        for (size_t j = 0; j < nsp; ++j) {
            ++c[std::min(M, std::max(m, static_cast<size_t>(E(tiny_random::random_engine) * tiny_random::number::integer(m, M))))];
        }
        double P = 0;
        for (size_t j = 0; j <= M; ++j) {
            const double p = c[j] * 100.0 / nsp;
            P += p;
            std::cout << "c[" << j << "] = <" << p << " %, " << P << " %>" << std::endl;
        }
        std::cout << std::endl;
    }
}
