// std
#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <set>
#include <unordered_set>

// Qt
#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

// googletest
#include <gtest/gtest.h>

// this software
#include "tiny_random.h"

// Modules tested
#include "../src/Algorithm.h"
#include "../src/FileSystemAccessor.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    GTEST_SKIP();
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
    EXPECT_EQ(7 * 6, 43);
}

// Tests for tests begins here
TEST(TestAlgorithm, Mod) {
    GTEST_SKIP();
    std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INT_MIN, INT_MAX);
    //std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
    const auto m = INT_MIN;
    //const auto m = INTMAX_MIN;
    //const auto a = 1, b = 256;
    const auto a = 0x20, b = 0x7E;
    const auto L = b - a + 1;
    for (size_t i = 0; i < 1e9; ++i) {
        const intmax_t x = max_uniform_int_dist(random_engine);
        const intmax_t A = (x - m) % L;
        const intmax_t B = (x % L - m % L) % L;
        EXPECT_EQ(A, B);
    }
}

TEST(TestAlgorithm, Random) {
    GTEST_SKIP();
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
        for (size_t i = 0; i < 1 * 1e9; ++i) {
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
        //const std::string s = tiny_random::chr::ASCII_string(l, t);
        //std::cout << s << std::endl;
        //return s;
    };

    constexpr size_t lmax = 256;    // max length of test strings
    for (size_t i = 0; i < 1e5; ++i) {
        const QByteArray s = QByteArray::fromStdString(next_str(next_int(1ull, lmax)));
        //const QByteArray s = "deuhf8uehfiueh8t98tu3489tue98jeoifgoidjgieyt894utioemfodgoieti84eutsfiuofsoijfsojfosjfosfsdi0";
        std::cout << s.toStdString() << std::endl;
    }
}

// Tests of WMM begin here.
TEST(Algorithm, StringIeq) { // ieq is from powershell
    namespace wmm = WritingMaterialsManager;
    
    auto next_int = [](const auto a, const auto b) -> auto {
        return tiny_random::number::integer(a, b);
    };
    auto next_str = [](const size_t l, const tiny_random::chr::ASCII_char_type t = tiny_random::chr::ASCII_char_type::printable) {
        return tiny_random::chr::ASCII_string(l, t);
    };

    { // csae-insensitive hasher
        constexpr size_t g = 1e4;       // group count of test data
        constexpr size_t lmax = 256;    // max length of test strings

        constexpr wmm::CaseInsensitiveHasher hasher;
        for (size_t i = 0; i < g; ++i) {
            const QByteArray s = QByteArray::fromStdString(next_str(next_int(1ull, lmax))), t = next_int(0, 1) ? s.toLower() : s.toUpper(); // 1ull -> 1uz since C++23
            const QString u = QString::fromStdString(next_str(next_int(1ull, lmax))), v = next_int(0, 1) ? u.toLower() : u.toUpper();
            const size_t h[2][2] = { { hasher(s), hasher(t) }, { hasher(u), hasher(v) } };
            for (size_t j = 0; j < 2; ++j) {
                EXPECT_EQ(h[j][0], h[j][1]);                                // s -ieq t -> H(s) == H(t), H is a hash function
                EXPECT_EQ(h[j][0], h[j][0]); EXPECT_EQ(h[j][1], h[j][1]);   // s -ceq t -> H(s) == H(t)
            }
            const QByteArray w = QByteArray::fromStdString(next_str(next_int(1ull, lmax))), x = QByteArray::fromStdString(next_str(next_int(1ull, lmax)));
            const QString y = QString::fromStdString(next_str(next_int(1ull, lmax))), z = QString::fromStdString(next_str(next_int(1ull, lmax)));
            if (w.toUpper() != x.toUpper()) { EXPECT_NE(hasher(w), hasher(x)); } // s != t -> H(s) != H(t)
            else { EXPECT_EQ(hasher(w), hasher(x)); }
            if (y.toUpper() != z.toUpper()) { EXPECT_NE(hasher(y), hasher(z)); }
            else { EXPECT_EQ(hasher(y), hasher(z)); }
        }
    }
    { // case-insensitive comparator
        constexpr size_t g = 1e2;       // group count of test data
        constexpr size_t lmax = 256;    // max length of test strings

        constexpr wmm::CaseInsensitiveStringComparator comparator;
        for (size_t i = 0; i < g; ++i) {
            const QString s = QString::fromStdString(next_str(next_int(1ull, lmax))), t = next_int(0, 1) ? s.toLower() : s.toUpper(); // haphazardly select tolower or toupper
            EXPECT_TRUE(comparator(s, t)); EXPECT_TRUE(comparator(s, s)); EXPECT_TRUE(comparator(t, t));
            const QString u = QString::fromStdString(next_str(next_int(1ull, lmax))), v = QString::fromStdString(next_str(next_int(1ull, lmax)));
            if (u.toUpper() != v.toUpper()) { EXPECT_FALSE(comparator(u, v)); }
            else { EXPECT_FALSE(comparator(u, v)); }
        }
    }
}

TEST(FileSystemAccessor, Read) {
    using fsa = WritingMaterialsManager::FileSystemAccessor;

    // create test files
    std::filesystem::create_directory("test");
    std::filesystem::create_directory("test/FileSystemAccessor");
    const std::string pwd = std::filesystem::absolute(std::filesystem::path("test/FileSystemAccessor")).string();

    constexpr size_t N = 1024;                      // number of test files
    constexpr size_t Lu = 16;                       // u means unit
    constexpr size_t Lmin = Lu, Lmax = 8ull << 20;  // min/max length of test files

    std::set<uintmax_t> basenames;
    std::unordered_set<QByteArray> contents;

    // regular file
    std::mt19937_64& R = tiny_random::random_engine;
    std::exponential_distribution E(0.001);
    for (size_t i = 0; i < N; ++i) {
        const uintmax_t basename = tiny_random::number::integer();
        std::ofstream f(pwd + '/' + std::to_string(basename) + ".txt", std::ios::out | std::ios::trunc | std::ios::binary);
        basenames.emplace(basename);
        const size_t L = std::min(Lmax, std::max(Lmin, static_cast<size_t>(Lu * E(R)))); // most files are small and a few of them are big.
        QByteArray content;
        for (size_t j = 0; j < L; j += sizeof(uintmax_t)) {
            const uintmax_t n[2] = { tiny_random::number::integer(), 0 };
            const char* const raw = reinterpret_cast<const char*>(n);
            content.append(QByteArray::fromRawData(raw, sizeof(uintmax_t)));
        }
        f.write(content.constData(), content.size());
        contents.emplace(content);
    }

    // interface test: read
    for (const auto& basename: basenames) { // regular files
        const std::shared_ptr<QFile> f = fsa::Open(QString::fromLocal8Bit(pwd + '/' + std::to_string(basename) + ".txt"));
        const std::shared_ptr<QFileInfo> fi = fsa::GetFileInfo(f);
        EXPECT_TRUE(basenames.contains(fi->baseName().toULongLong()));
        EXPECT_TRUE(contents.contains(fsa::GetAllRawContents(f)));
    }
    bool has_open_exception = false; // open exception test
    try {
        fsa::Open(QString::fromLocal8Bit(pwd+"/open-exception-test.t"));
    }
    catch (const std::runtime_error& e) {
        has_open_exception = true;
        EXPECT_EQ(std::string(e.what()).substr(0, 10), std::string("Open file "));
    }
    EXPECT_TRUE(has_open_exception);
}
