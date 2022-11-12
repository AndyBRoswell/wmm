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
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// googletest
#include <gtest/gtest.h>

// this software
#include "tiny_random.h"

// Modules tested
#include "src/Algorithm.h"
#include "src/FileSystemAccessor.h"
#include "src/JSONFormatter.h"
#include "src/MongoDBAccessor.h"

constexpr auto next_int = [](const auto a, const auto b) noexcept -> auto {
    return tiny_random::number::integer(a, b);
};
constexpr auto next_str = [](const size_t l, const tiny_random::chr::ASCII_char_type t = tiny_random::chr::ASCII_char_type::printable) noexcept {
    return tiny_random::chr::ASCII_string(l, t);
};

// Tests of WMM begin here.
TEST(Algorithm, CaseInsensitiveHasher) {
    namespace wmm = WritingMaterialsManager;

    constexpr size_t g = 1e6;       // group count of test data
    constexpr size_t lmax = 1e3;    // max length of test strings

    constexpr wmm::CaseInsensitiveHasher hasher;
    for (size_t i = 0; i < g; ++i) {
        {
            enum class Type { const_char_star, QByteArrayView, QLatin1StringView, QAnyStringView, };

            const std::vector<Type> types{ Type::const_char_star, Type::QByteArrayView, Type::QLatin1StringView, Type::QAnyStringView, };
            for (const auto T: types) {
                std::array<std::string, 3> s;
                std::generate(s.begin(), s.end(), []() { return next_str(next_int(1ull, lmax)); });

                std::string t[3] = { s[0], s[1], s[2] };
                next_int(0, 1) ? std::transform(t[0].cbegin(), t[0].cend(), t[0].begin(), ::toupper) : std::transform(t[0].cbegin(), t[0].cend(), t[0].begin(), ::tolower); // haphazardly select tolower or toupper
                std::array<size_t, 2> H;
                switch (T) {
                case Type::const_char_star: H = { hasher(s[0].c_str()), hasher(t[0].c_str()) }; break;
                case Type::QByteArrayView: H = { hasher(QByteArrayView(s[0].c_str())), hasher(QByteArrayView(t[0].c_str())) }; break;
                case Type::QLatin1StringView: H = { hasher((QLatin1StringView(s[0].c_str()))), hasher(QLatin1StringView(t[0].c_str())) }; break;
                case Type::QAnyStringView: H = { hasher(QAnyStringView(s[0].c_str())), hasher(QAnyStringView(t[0].c_str())) }; break;
                }
                EXPECT_EQ(H[0], H[1]);                          // s -ieq t -> H(s) == H(t), H is a hash function, t = s.toUpper()
                EXPECT_EQ(H[0], H[0]); EXPECT_EQ(H[1], H[1]);   // s -ceq t -> H(s) == H(t)
                for (size_t i = 1; i <= 2; ++i) { std::transform(t[i].cbegin(), t[i].cend(), t[i].begin(), ::toupper); } // t[i] = s[i].toUpper()
                if (t[1] != t[2]) { EXPECT_NE(hasher(t[1].c_str()), hasher.operator()(t[2].c_str())); } // It is almost inevitable that s[1] != s[2], then t[1] != t[2]
                else { EXPECT_EQ(hasher(t[1].c_str()), hasher.operator()(t[2].c_str())); }
            }
        }
        {
            enum class Type{ const_char8_t_star, QUtf8StringView, };

            const std::vector<Type> types{ Type::const_char8_t_star, Type::QUtf8StringView, };
            for (const auto T : types) {
                std::vector<std::u8string> s(3);
                for (auto& str : s) {
                    const auto t = next_str(next_int(1ull, lmax));
                    str.reserve(t.size());
                    for (const auto c : t) { str.push_back(c); }
                }

                std::u8string t[3] = { s[0], s[1], s[2] };
                next_int(0, 1) ? std::transform(t[0].cbegin(), t[0].cend(), t[0].begin(), ::toupper) : std::transform(t[0].cbegin(), t[0].cend(), t[0].begin(), ::tolower); // haphazardly select tolower or toupper
                std::array<size_t, 2> H;
                switch (T) {
                case Type::const_char8_t_star: H = { hasher(s[0].c_str()), hasher(t[0].c_str()) }; break;
                case Type::QUtf8StringView: H = { hasher(QUtf8StringView(s[0].c_str())), hasher(QUtf8StringView(t[0].c_str())) }; break;
                }
                EXPECT_EQ(H[0], H[1]);                          // s -ieq t -> H(s) == H(t), H is a hash function, t = s.toUpper()
                EXPECT_EQ(H[0], H[0]); EXPECT_EQ(H[1], H[1]);   // s -ceq t -> H(s) == H(t)
                for (size_t i = 1; i <= 2; ++i) { std::transform(t[i].cbegin(), t[i].cend(), t[i].begin(), ::toupper); } // t[i] = s[i].toUpper()
                if (t[1] != t[2]) { EXPECT_NE(hasher(t[1].c_str()), hasher.operator()(t[2].c_str())); } // It is almost inevitable that s[1] != s[2], then t[1] != t[2]
                else { EXPECT_EQ(hasher(t[1].c_str()), hasher.operator()(t[2].c_str())); }
            }
        }
        {
            enum class Type { QByteArray, QString, };

            auto verify = []<class T>(const std::array<T, 3>&s) {
                static constexpr wmm::CaseInsensitiveHasher hasher;

                const T t = next_int(0, 1) ? s[0].toUpper() : s[0].toLower();
                const size_t H[2] = { hasher(s[0]), hasher(t) };
                EXPECT_EQ(H[0], H[1]);                          // s -ieq t -> H(s) == H(t), H is a hash function, t = s.toUpper()
                EXPECT_EQ(H[0], H[0]); EXPECT_EQ(H[1], H[1]);   // s -ceq t -> H(s) == H(t)
                if (s[1].toUpper() != s[2].toUpper()) { EXPECT_NE(hasher(s[1]), hasher(s[2])); } // // It is almost inevitable that s[1] != s[2], then s[1].toUpper() != s[2].toUpper()
                else { EXPECT_EQ(hasher(s[1]), hasher(s[2])); }
            };

            const std::vector<Type> types{ Type::QByteArray, Type::QString, };
            for (const auto T : types) {
                switch (T) {
                case Type::QByteArray: {
                    std::array<QByteArray, 3> s;
                    std::generate(s.begin(), s.end(), []() { return QByteArray::fromStdString(next_str(next_int(1ull, lmax))); });
                    verify(s);
                } break;
                case Type::QString: {
                    std::array<QString, 3> s;
                    std::generate(s.begin(), s.end(), []() { return QString::fromStdString(next_str(next_int(1ull, lmax))); });
                    verify(s);
                } break;
                }
            }
        }
    }
}

TEST(Algorithm, CaseInsensitiveComparator) {
    namespace wmm = WritingMaterialsManager;

    constexpr size_t g = 1e5;       // group count of test data
    constexpr size_t lmax = 1e3;    // max length of test strings

    constexpr wmm::CaseInsensitiveStringComparator comparator;
    for (size_t i = 0; i < g; ++i) { // verify the comparator
        {
            std::vector<std::string> s(3);
            std::generate(s.begin(), s.end(), []() { return next_str(next_int(1ull, lmax)); });

            std::string t[3] = { s[0], s[1], s[2] };
            next_int(0, 1) ? std::transform(t[0].cbegin(), t[0].cend(), t[0].begin(), ::toupper) : std::transform(t[0].cbegin(), t[0].cend(), t[0].begin(), ::tolower); // haphazardly select tolower or toupper
            EXPECT_TRUE(comparator(s[0].c_str(), t[0].c_str()));  // this should yield equal because 2 strings are equal when ignoring the case
            EXPECT_TRUE(comparator(s[0].c_str(), s[0].c_str()));  // comparison of 2 identical strings should yield equal
            EXPECT_TRUE(comparator(t[0].c_str(), t[0].c_str()));
            for (size_t i = 1; i <= 2; ++i) { std::transform(t[i].cbegin(), t[i].cend(), t[i].begin(), ::toupper); }
            if (t[1] != t[2]) { EXPECT_FALSE(comparator(s[1].c_str(), s[2].c_str())); } // these 2 strings converted to the same case are not identical
            else { EXPECT_TRUE(comparator(s[1].c_str(), s[2].c_str())); }
        }
        {
            std::vector<QByteArray> s(3);
            std::generate(s.begin(), s.end(), []() { return QByteArray::fromStdString(next_str(next_int(1ull, lmax))); });

            const QByteArray t = next_int(0, 1) ? s[0].toUpper() : s[0].toLower();
            EXPECT_TRUE(comparator(s[0], t)); EXPECT_TRUE(comparator(s[0], s[0])); EXPECT_TRUE(comparator(t, t));
            if (s[1].toUpper() != s[2].toUpper()) { EXPECT_FALSE(comparator(s[1], s[2])); }
            else { EXPECT_TRUE(comparator(s[1], s[2])); }
        }
        {
            std::vector<QString> s(3);
            std::generate(s.begin(), s.end(), []() { return QString::fromStdString(next_str(next_int(1ull, lmax))); });

            const QString t = next_int(0, 1) ? s[0].toUpper() : s[0].toLower();
            EXPECT_TRUE(comparator(s[0], t)); EXPECT_TRUE(comparator(s[0], s[0])); EXPECT_TRUE(comparator(t, t));
            if (s[1].toUpper() != s[2].toUpper()) { EXPECT_FALSE(comparator(s[1], s[2])); }
            else { EXPECT_TRUE(comparator(s[1], s[2])); }
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
    constexpr size_t Lmin = Lu, Lmax = 16ull << 20; // min/max length of test files

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

    GTEST_SKIP();
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

    GTEST_SKIP();
    for (const auto& basename : basenames) { // format
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
    WritingMaterialsManager::MongoDBAccessor mongoa;
    const QByteArray information[] = {
        mongoa.GetDatabasesInformation(),
        mongoa.GetCollectionsInformation("admin"), // these 3 databases are built-in
        mongoa.GetCollectionsInformation("config"),
        mongoa.GetCollectionsInformation("local"),
        mongoa.GetDBsAndCollsInfo(),
    };
    for (const auto& i : information) {
        QJsonParseError e;
        const auto d = QJsonDocument::fromJson(i, &e);
        EXPECT_EQ(e.error, QJsonParseError::ParseError::NoError);   // prove that the returned JSON exactly has NO lexical/syntax error
        EXPECT_TRUE(d.array()[0].toObject().contains("name"));      // and the expected basic information
        //std::cout << i.toStdString() << "\n" << std::endl;
    }
}
