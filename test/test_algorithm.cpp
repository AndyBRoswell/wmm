// std
#include <chrono>
#include <random>

// Qt
#include <QByteArray>
#include <QJsonDocument>

// googletest
#include <gtest/gtest.h>

// this software
#include "tiny_random.h"

// Tests for tests begins here
TEST(TestAlgorithm, Rem) {
    GTEST_SKIP();
    std::mt19937_64& random_engine = tiny_random::random_engine;
    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INT_MIN, INT_MAX);
    constexpr auto m = INT_MIN;
    constexpr auto a = 0x20, b = 0x7E;
    constexpr auto L = b - a + 1;
    for (size_t i = 0; i < 1e3; ++i) {
        const intmax_t x = max_uniform_int_dist(random_engine);
        const intmax_t A = (x - m) % L;
        const intmax_t B = (x % L - m % L) % L;
        EXPECT_EQ(A, B); // operator% doesn't comply this property
    }
}

TEST(TestAlgorithm, Mod) {
    using tiny_random::number::mod;

    std::mt19937_64& R = tiny_random::random_engine;
    std::uniform_int_distribution<intmax_t> U(INTMAX_MIN / 2, INTMAX_MAX / 2); // limit the range of random numbers to avert overflow
    std::uniform_int_distribution<intmax_t> u(INT_MIN, INT_MAX);
    { // a very simple demo of overflow
        constexpr intmax_t x[][3] = { { 10, -20, -1e10 }, { -1e6, -2e6, -1e10 }, { 1e9, 2e9, -5e9 }, { 1e9, 2e9, -1e10 }, };
        constexpr bool expect_eq[] = { true, true, false, false, };
        for (size_t i = 0; i < sizeof(x) / (3 * sizeof(intmax_t)); ++i) {
            const auto a = x[i][0], b = x[i][1], c = x[i][2];
            const intmax_t A = mod(a * b, c), B = mod(mod(a, c) * mod(b, c), c);
            if (expect_eq[i]) { EXPECT_EQ(A, B); }
            else { EXPECT_NE(A, B); }
        }
    }
    constexpr size_t n = 1e9; // test count
    for (size_t i = 0; i < n; ++i) {
        auto a = U(R), b = U(R), c = U(R);
        while (c == 0) { c = U(R); } // division by zero is not allowed
        EXPECT_EQ(mod(a + b, c), mod(mod(a, c) + mod(b, c), c));
        EXPECT_EQ(mod(a - b, c), mod(mod(a, c) - mod(b, c), c));
        a = u(R), b = u(R), c = u(R);
        while (c == 0) { c = u(R); } // division by zero is not allowed
        EXPECT_EQ(mod(a * b, c), mod(mod(a, c) * mod(b, c), c));
    }
}

TEST(TestAlgorithm, Integer) {
    using tiny_random::number::integer;

    constexpr size_t n = 20; // test count    

    { // integer()
        std::vector<intmax_t> s{ INTMAX_MIN }; std::vector<uintmax_t> t{ 0 };
        for (size_t i = 0; i < n; ++i) {
            s.emplace_back(integer<intmax_t>()); t.emplace_back(integer());
        }
        s.emplace_back(INTMAX_MAX); t.emplace_back(UINTMAX_MAX);
        std::sort(s.begin(), s.end()); std::sort(t.begin(), t.end());
        for (size_t i = 0; i <= n + 1; ++i) {
            std::cout << s[i] << "\t\t\t\t" << t[i] << std::endl;
        }
        std::cout << std::endl;

        std::vector<intmax_t> u, v;
        double EDs = 0, EDt = 0; // E = expectation
        double EDu = 0, EDv = 0;
        for (size_t i = 1; i <= n + 1; ++i) {
            const auto ds = s[i] - s[i - 1]; const auto dt = t[i] - t[i - 1]; // >= 0
            EDs += ds; EDt += dt;
            u.emplace_back(ds); v.emplace_back(dt);
        }
        for (size_t i = 0; i <= n; ++i) {
            std::cout << u[i] << "\t\t\t\t" << v[i] << std::endl;
        }
        std::cout << std::endl;
        EDs /= n + 1; EDt /= n + 1; 
        const double EE = pow(2, 64) / (n + 1);
        for (size_t i = 0; i <= n; ++i) {
            EDu += abs(u[i] - EDs); EDv += abs(v[i] - EDt);
        }
        EDu /= n + 1; EDv /= n + 1;
        const auto w = std::minmax_element(s.cbegin(), s.cend());
        const auto x = std::minmax_element(t.cbegin(), t.cend());
        const auto y = std::minmax_element(u.cbegin(), u.cend());
        const auto z = std::minmax_element(v.cbegin(), v.cend());
        std::cout << (EDs - EE) / EE * 1e6 << " ppm" << std::endl;
        std::cout << (EDt - EE) / EE * 1e6 << " ppm" << std::endl;
        std::cout << EDu << std::endl;
        std::cout << EDv << std::endl;
        std::cout << *w.first << "\t\t\t\t" << *w.second << std::endl;
        std::cout << *x.first << "\t\t\t\t" << *x.second << std::endl;
        std::cout << *y.first << "\t\t\t\t" << *y.second << std::endl;
        std::cout << *z.first << "\t\t\t\t" << *z.second << std::endl;
    }

    // <class T> integer(const T, const T)

    // ASCII

    // ASCII_string
}

TEST(TestAlgorithm, JSON) {

}

TEST(TestAlgorithm, Random) {
    auto mod = [](const auto& N, const auto& D) constexpr {
        const auto r = N % D;
        if (D > 0) { return r >= 0 ? r : r + D; }
        else { return r <= 0 ? r : r + D; }
    };
    std::mt19937_64& random_engine = tiny_random::random_engine;
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
    GTEST_SKIP();
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
