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

    { // integer()
        constexpr size_t n = 1e3; // test count

        size_t nonneg = 0, neg = 0;
        while (nonneg < n || neg < n) {
            const auto x = integer<intmax_t>();
            x >= 0 ? ++nonneg : ++neg;
        }
        const auto x = integer();
        static_assert(std::is_same_v<std::remove_const_t<decltype(x)>, uintmax_t>);
    }
    { // <class T> integer(const T, const T)
        constexpr size_t N = 1e3; // test count
        constexpr size_t n = 1e6; // sub-test count

        std::mt19937_64& RE = tiny_random::random_engine;
        std::uniform_int_distribution<intmax_t> US(INTMAX_MIN / 2, INTMAX_MAX / 2);
        std::uniform_int_distribution<uintmax_t> UU(0, UINTMAX_MAX);
        for (size_t i = 0; i < N; ++i) {
            const auto p = std::minmax(US(RE), US(RE));
            const auto m = p.first, M = p.second;
            for (size_t j = 0; j < n; ++j) {
                const auto x = integer(m, M);
                EXPECT_GE(x, m); EXPECT_LE(x, M);
            }
            const auto q = std::minmax(UU(RE), UU(RE));
            const auto mu = q.first, Mu = q.second;
            for (size_t j = 0; j < n; ++j) {
                const auto x = integer(mu, Mu);
                EXPECT_GE(x, mu); EXPECT_LE(x, Mu);
            }
        }
        { // special cases
            constexpr intmax_t b[][2] = { // B/b means bound
                { 0, 0 } , { -1, 0 }, { 0, 1 }, { -1, 1 }, { -2, 1 }, { -1, 2 },
                { INTMAX_MIN / 2, INTMAX_MIN / 2 }, { INTMAX_MAX / 2, INTMAX_MAX / 2 }, { INTMAX_MIN / 2 + 1, INTMAX_MAX / 2 }, 
            };
            constexpr uintmax_t B[][2] = {
                { 0, 0 }, { 1, 1 },
                { UINTMAX_MAX, UINTMAX_MAX }, { 0, UINTMAX_MAX },
            };
            for (size_t i = 0; i < sizeof(b) / (2 * sizeof(intmax_t)); ++i) {
                for (size_t j = 0; j < n; ++j) {
                    const auto m = b[i][0], M = b[i][1];
                    const auto x = integer(m, M);
                    EXPECT_GE(x, m); EXPECT_LE(x, M);
                }
            }
            for (size_t i = 0; i < sizeof(B) / (2 * sizeof(uintmax_t)); ++i) {
                for (size_t j = 0; j < n; ++j) {
                    const auto mu = b[i][0], Mu = b[i][1];
                    const auto x = integer(mu, Mu);
                    EXPECT_GE(x, mu); EXPECT_LE(x, Mu);
                }
            }
        }
    }
    { // ASCII
        using namespace tiny_random;
        using tiny_random::chr::ASCII_char_type;
        using tiny_random::chr::ASCII;
        using tiny_random::chr::ASCII_string;
        using enum ASCII_char_type;

        constexpr size_t n = 1e3; // test count

        const std::set<int> shex(chr::hex, chr::hex + sizeof(chr::hex) - 1);
        const std::set<int> slhex(chr::lhex, chr::lhex + sizeof(chr::lhex) - 1);
        const std::set<int> sualnum(chr::ualnum, chr::ualnum + sizeof(chr::ualnum) - 1);
        const std::set<int> slalnum(chr::lalnum, chr::lalnum + sizeof(chr::lalnum) - 1);
        const std::vector<ASCII_char_type> enum_to_test = { dec, hex, lhex, ucase, lcase, alpha, ualnum, lalnum, alnum, punct, printable, };
        const std::vector<std::tuple<std::function<bool(int)>, std::set<int>>> f = {
            { [](int c) { return isdigit(c); }, {}, },
            { [&shex](int c) { return shex.contains(c); }, {}, },
            { [&slhex](int c) { return slhex.contains(c); }, {}, },
            { [](int c) { return isupper(c); }, {}, },
            { [](int c) { return islower(c); }, {}, },
            { [](int c) { return isalpha(c); }, {}, },
            { [&sualnum](int c) { return sualnum.contains(c); }, {}, },
            { [&slalnum](int c) { return slalnum.contains(c); }, {}, },
            { [](int c) { return isalnum(c); }, {}, },
            { [](int c) { return ispunct(c); }, {}, },
            { [](int c) { return c >= 0x20 and c <= 0x7e; }, {}, },
        };

        for (const auto& i : enum_to_test) {

        }
    }
}

TEST(TestAlgorithm, JSON) {

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

TEST(TestAlgorithm, Deviation) {
    GTEST_SKIP();
    using tiny_random::number::integer;

    constexpr size_t n = 1e8; // test count    

    std::vector<intmax_t> s{ INTMAX_MIN }; std::vector<uintmax_t> t{ 0 };
    for (size_t i = 0; i < n; ++i) {
        s.emplace_back(integer<intmax_t>()); t.emplace_back(integer());
    }
    s.emplace_back(INTMAX_MAX); t.emplace_back(UINTMAX_MAX);
    std::sort(s.begin(), s.end()); std::sort(t.begin(), t.end());

    std::vector<intmax_t> ds; std::vector<intmax_t> dt;
    const double Ed = pow(2, 64) / (n + 1); // theorectical mean of distance
    double Eds = 0, Edt = 0; // E = expectation
    double EDds = 0, EDdt = 0; // D = deviation
    for (size_t i = 1; i <= n + 1; ++i) {
        const auto d0 = s[i] - s[i - 1]; const auto d1 = t[i] - t[i - 1]; // >= 0
        Eds += d0; Edt += d1;
        ds.emplace_back(d0); dt.emplace_back(d1);
    }
    Eds /= n + 1; Edt /= n + 1;
    for (size_t i = 0; i <= n; ++i) {
        EDds += abs(ds[i] - Ed); EDdt += abs(dt[i] - Ed);
    }
    EDds /= n + 1; EDdt /= n + 1;

    std::cout << Ed << std::endl;
    std::cout << Eds << std::endl;
    std::cout << Edt << std::endl;
    std::cout << (Eds - Ed) / Ed * 1e6 << " ppm" << std::endl;
    std::cout << (Edt - Ed) / Ed * 1e6 << " ppm" << std::endl;
    std::cout << EDds << std::endl;
    std::cout << EDdt << std::endl;
    std::cout << (EDds - Ed) / Ed * 1e6 << " ppm" << std::endl;
    std::cout << (EDdt - Ed) / Ed * 1e6 << " ppm" << std::endl;

    const auto p = std::minmax_element(ds.cbegin(), ds.cend());
    const auto q = std::minmax_element(dt.cbegin(), dt.cend());
    std::cout << *p.first << "\t\t\t\t" << *p.second << std::endl;
    std::cout << *q.first << "\t\t\t\t" << *q.second << std::endl;
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
