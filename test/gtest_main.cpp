// std
#include <chrono>
#include <filesystem>
#include <fstream>
#include <numbers>
#include <random>
#include <set>
#include <unordered_set>

// Qt
#include <QByteArray>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QString>

// googletest
#include <gtest/gtest.h>

// this software
#include "tiny_random.h"

// Modules tested
#include "../src/Algorithm.h"
#include "../src/FileSystemAccessor.h"
#include "../src/JSONFormatter.h"
#include "../src/MongoDBAccessor.h"

// Tests of WMM begin here.
TEST(Algorithm, StringIeq) { // ieq is from powershell
    namespace wmm = WritingMaterialsManager;
    
    constexpr auto next_int = [](const auto a, const auto b) -> auto {
        return tiny_random::number::integer(a, b);
    };
    constexpr auto next_str = [](const size_t l, const tiny_random::chr::ASCII_char_type t = tiny_random::chr::ASCII_char_type::printable) {
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
    try { fsa::Open(QString::fromLocal8Bit(pwd+"/open-exception-test.t")); } // deliberately open a non-existing file
    catch (const std::runtime_error& e) {
        has_open_exception = true;
        EXPECT_EQ(std::string(e.what()).substr(0, 10), std::string("Open file "));
    }
    EXPECT_TRUE(has_open_exception);
}

TEST(JSONFormatter, Default) {
    using fsa = WritingMaterialsManager::FileSystemAccessor;

    // create test files
    std::filesystem::create_directory("test");
    std::filesystem::create_directory("test/JSON");
    const std::string pwd = std::filesystem::absolute(std::filesystem::path("test/JSON")).string();

    constexpr size_t N = 10; // number of test files
    
    std::set<uintmax_t> basenames;
    for (size_t i = 0; i < N; ++i) {
        const auto basename = tiny_random::number::integer();
        std::ofstream f(pwd + '/' + std::to_string(basename) + ".json", std::ios::out | std::ios::trunc);
        basenames.emplace(basename);
        const auto json = tiny_random::chr::JSON();
        QJsonParseError error;
        const QJsonDocument d = QJsonDocument::fromJson(QByteArray::fromStdString(json), &error);
        EXPECT_EQ(error.error, QJsonParseError::ParseError::NoError); // prove that the generated JSON has NO lexical/syntax error
        f << json;
        f.flush();
    }

    // format
    for (const auto& basename : basenames) {
        const auto f = fsa::Open(QString::fromStdString(pwd) + '/' + std::to_string(basename).c_str() + ".json", QIODevice::ReadOnly);
        auto content = fsa::GetAllRawContents(f);
        auto json = QString::fromUtf8(content);
        WritingMaterialsManager::JSONFormatter formatter;
        formatter.Format(json);
        const auto g = fsa::Open(QString::fromStdString(pwd) + '/' + std::to_string(basename).c_str() + "F.json", QIODevice::WriteOnly);
        g->write(json.toStdString().c_str());
        const auto h = fsa::Open(QString::fromStdString(pwd) + '/' + std::to_string(basename).c_str() + "F.json", QIODevice::ReadOnly);
        EXPECT_EQ(content, fsa::GetAllRawContents(h));
    }
}

TEST(MongoDBAccessor, BasicInfo) {
    WritingMaterialsManager::MongoDBAccessor a;
    std::cout << a.GetDatabasesInformation().toStdString() << "\n" << std::endl
        << a.GetCollectionsInformation(QByteArray("admin")).toStdString() << "\n" << std::endl
        << a.GetCollectionsInformation("config").toStdString() << "\n" << std::endl
        << a.GetCollectionsInformation("local").toStdString() << "\n" << std::endl
        << a.GetDBsAndCollsInfo().toStdString() << std::endl;
}
