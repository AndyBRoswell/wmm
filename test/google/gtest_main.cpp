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
constexpr auto random_case = []<class T>(const T & s) noexcept {
    T t;
    t.reserve(s.size());
    if constexpr (std::is_same_v<T, QString>) { for (const auto c : s) { next_int(0, 1) == 0 ? t.push_back(c.toUpper()) : t.push_back(c.toLower()); } }
    else { for (const auto c : s) { next_int(0, 1) == 0 ? t.push_back(toupper(c)) : t.push_back(tolower(c)); } }
    return t;
};

// Tests of WMM begin here.
TEST(Algorithm, CaseInsensitiveHasher) {
    namespace wmm = WritingMaterialsManager;

    constexpr size_t g = 1e6;       // group count of test data
    constexpr size_t lmax = 1e3;    // max length of test strings

    for (size_t i = 0; i < g; ++i) {
        { // string views and primitive char arrays
            enum class Type { const_char_star, QByteArrayView, QLatin1StringView, QAnyStringView, const_char8_t_star, QUtf8StringView, QStringView, };

            static constexpr wmm::CaseInsensitiveHasher hasher;
            auto verify = [&]<class Ty>(std::array<Ty, 3>&s, const std::array<size_t, 2>&H) {
                EXPECT_EQ(H[0], H[1]);                          // s -ieq t -> H(s) == H(t), H is a hash function, t = s.toUpper()
                EXPECT_EQ(H[0], H[0]); EXPECT_EQ(H[1], H[1]);   // s -ceq t -> H(s) == H(t)
                std::array<Ty, 2> t;
                for (size_t i = 1; i <= 2; ++i) {
                    t[i - 1].reserve(s[i].size());
                    for (const auto c : s[i]) { t[i - 1].push_back(toupper(c)); }
                }
                if (t[0] != t[1]) { EXPECT_NE(hasher(s[1].c_str()), hasher(s[2].c_str())); } // It is almost inevitable that s[1] != s[2], then s[1].toUpper() != s[2].toUpper()
                else { EXPECT_EQ(hasher(s[1].c_str()), hasher(s[2].c_str())); }
            };

            constexpr Type types[] = { Type::const_char_star, Type::QByteArrayView, Type::QLatin1StringView, Type::QAnyStringView, Type::const_char8_t_star, Type::QUtf8StringView, Type::QStringView, };
            for (const auto T : types) {
                switch (T) {
                case Type::const_char_star: case Type::QByteArrayView: case Type::QLatin1StringView: case Type::QAnyStringView: {
                    std::array<std::string, 3> s;
                    std::generate(s.begin(), s.end(), []() { return next_str(next_int(1ull, lmax)); });

                    const std::string t = random_case(s[0]);
                    std::array<size_t, 2> H;
                    switch (T) {
                    case Type::const_char_star: H = { hasher(s[0].c_str()), hasher(t.c_str()) }; break;
                    case Type::QByteArrayView: H = { hasher(QByteArrayView(s[0].c_str())), hasher(QByteArrayView(t.c_str())) }; break;
                    case Type::QLatin1StringView: H = { hasher((QLatin1StringView(s[0].c_str()))), hasher(QLatin1StringView(t.c_str())) }; break;
                    case Type::QAnyStringView: H = { hasher(QAnyStringView(s[0].c_str())), hasher(QAnyStringView(t.c_str())) }; break;
                    }
                    verify(s, H);
                } break;
                case Type::const_char8_t_star: case Type::QUtf8StringView: {
                    std::array<std::u8string, 3> s;
                    for (auto& str : s) {
                        const auto t = next_str(next_int(1ull, lmax));
                        str.reserve(t.size());
                        for (const auto c : t) { str.push_back(c); }
                    }
                    
                    const std::u8string t = random_case(s[0]);
                    std::array<size_t, 2> H;
                    switch (T) {
                    case Type::const_char8_t_star: H = { hasher(s[0].c_str()), hasher(t.c_str()) }; break;
                    case Type::QUtf8StringView: H = { hasher(QUtf8StringView(s[0].c_str())), hasher(QUtf8StringView(t.c_str())) }; break;
                    }
                    verify(s, H);
                } break;
                case Type::QStringView: {
                    std::array<QString, 3> s;
                    std::generate(s.begin(), s.end(), []() { return QString::fromStdString(next_str(next_int(1ull, lmax))); });

                    const QString t = random_case(s[0]);
                    const std::array<size_t, 2> H = { hasher(QStringView(s[0].constData())), hasher(QStringView(t.constData())) };
                    EXPECT_EQ(H[0], H[1]);                          // s -ieq t -> H(s) == H(t), H is a hash function, t = s.toUpper()
                    EXPECT_EQ(H[0], H[0]); EXPECT_EQ(H[1], H[1]);   // s -ceq t -> H(s) == H(t)
                    if (s[1].toUpper() != s[2].toUpper()) { EXPECT_NE(hasher(QStringView(s[1].constData())), hasher(QStringView(s[2].constData()))); } // It is almost inevitable that s[1] != s[2], then t[1] != t[2]
                    else { EXPECT_EQ(hasher(QStringView(s[1].constData())), hasher(QStringView(s[2].constData()))); }
                } break;
                }
            }
        }
        { // objects instead of string views or primitive char arrays
            enum class Type { QByteArray, QString, };

            static constexpr wmm::CaseInsensitiveHasher hasher;
            auto verify = [&]<class T>(const std::array<T, 3>&s) {
                const T t = random_case(s[0]);
                const size_t H[2] = { hasher(s[0]), hasher(t) };
                EXPECT_EQ(H[0], H[1]);                          // s -ieq t -> H(s) == H(t), H is a hash function, t = s.toUpper()
                EXPECT_EQ(H[0], H[0]); EXPECT_EQ(H[1], H[1]);   // s -ceq t -> H(s) == H(t)
                if (s[1].toUpper() != s[2].toUpper()) { EXPECT_NE(hasher(s[1]), hasher(s[2])); } // // It is almost inevitable that s[1] != s[2], then s[1].toUpper() != s[2].toUpper()
                else { EXPECT_EQ(hasher(s[1]), hasher(s[2])); }
            };

            constexpr Type types[] = { Type::QByteArray, Type::QString, };
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

    for (size_t i = 0; i < g; ++i) { // verify the comparator
        { // string views and primitive char arrays
            enum class Type { 
                const_char_star, QByteArrayView, QLatin1StringView, QAnyStringView, 
                const_char8_t_star, QUtf8StringView,
                const_char16_t_star, const_uint16_t_star, const_QChar_star, QStringView, 
            };

            static constexpr wmm::CaseInsensitiveStringComparator comparator;
            auto verify = [&]<class StrT, class DestT = void>(const std::array<StrT, 3>&s) {
                std::array<StrT, 3> t = { random_case(s[0]), s[1], s[2]};
                if constexpr (std::is_same_v<DestT, void>) {
                    EXPECT_TRUE(comparator(s[0].c_str(), t[0].c_str()));  // this should yield equal because 2 strings are equal when ignoring the case
                    EXPECT_TRUE(comparator(s[0].c_str(), s[0].c_str()));  // comparison of 2 identical strings should yield equal
                    EXPECT_TRUE(comparator(t[0].c_str(), t[0].c_str()));
                    for (size_t i = 1; i <= 2; ++i) { std::transform(t[i].cbegin(), t[i].cend(), t[i].begin(), ::toupper); }
                    if (t[1] != t[2]) { EXPECT_FALSE(comparator(s[1].c_str(), s[2].c_str())); } // these 2 strings converted to the same case are not identical
                    else { EXPECT_TRUE(comparator(s[1].c_str(), s[2].c_str())); }
                }
                else {
                    EXPECT_TRUE(comparator(DestT(s[0].c_str()), DestT(t[0].c_str())));  // this should yield equal because 2 strings are equal when ignoring the case
                    EXPECT_TRUE(comparator(DestT(s[0].c_str()), DestT(s[0].c_str())));  // comparison of 2 identical strings should yield equal
                    EXPECT_TRUE(comparator(DestT(t[0].c_str()), DestT(t[0].c_str())));
                    for (size_t i = 1; i <= 2; ++i) { std::transform(t[i].cbegin(), t[i].cend(), t[i].begin(), ::toupper); }
                    if (t[1] != t[2]) { EXPECT_FALSE(comparator(DestT(s[1].c_str()), DestT(s[2].c_str()))); } // these 2 strings converted to the same case are not identical
                    else { EXPECT_TRUE(comparator(DestT(s[1].c_str()), DestT(s[2].c_str()))); }
                }
            };
            auto fill_non_std_string = []<class T>(T& str) {
                const auto t = next_str(next_int(1ull, lmax));
                str.reserve(t.size());
                for (const auto c : t) { str.push_back(c); }
            };

            constexpr Type types[] = { 
                Type::const_char_star, Type::QByteArrayView, Type::QLatin1StringView, Type::QAnyStringView, 
                Type::const_char8_t_star, Type::QUtf8StringView, 
                Type::const_char16_t_star, Type::const_uint16_t_star, Type::const_QChar_star, Type::QStringView,
            };
            for (const auto T : types) {
                switch (T) {
                case Type::const_char_star: case Type::QByteArrayView: case Type::QLatin1StringView: case Type::QAnyStringView: {
                    std::array<std::string, 3> s;
                    std::generate(s.begin(), s.end(), []() { return next_str(next_int(1ull, lmax)); });
                    switch (T) {
                    case Type::const_char_star: verify(s); break;
                    case Type::QByteArrayView: verify.operator() < std::string, QByteArrayView > (s); break;
                    case Type::QLatin1StringView: verify.operator() < std::string, QLatin1StringView > (s); break;
                    case Type::QAnyStringView: verify.operator() < std::string, QAnyStringView > (s); break;
                    }
                } break;
                case Type::const_char8_t_star: case Type::QUtf8StringView: {
                    std::array<std::u8string, 3> s;
                    for (auto& str : s) { fill_non_std_string(str); }
                    switch (T) {
                    case Type::const_char8_t_star: verify(s); break;
                    case Type::QUtf8StringView: verify.operator() < std::u8string, QUtf8StringView > (s); break;
                    }
                } break;
                case Type::const_char16_t_star: case Type::const_uint16_t_star: case Type::const_QChar_star: case Type::QStringView: {
                    std::array<std::u16string, 3> s;
                    for (auto& str : s) { fill_non_std_string(str); }
                    switch (T) {
                    case Type::const_char16_t_star: case Type::const_uint16_t_star: case Type::const_QChar_star: verify(s); break;
                    case Type::QStringView: verify.operator() < std::u16string, QStringView > (s); break;
                    }
                } break;
                }
            }
        }
        { // objects instead of string views or primitive char arrays
            enum class Type { QByteArray, QString, };

            static constexpr wmm::CaseInsensitiveStringComparator comparator;
            auto verify = [&]<class T>(const std::array<T, 3>&s) {
                const T t = next_int(0, 1) ? s[0].toUpper() : s[0].toLower();
                EXPECT_TRUE(comparator.operator() < T > (s[0], t)); EXPECT_TRUE(comparator.operator() < T > (s[0], s[0])); EXPECT_TRUE(comparator.operator() < T > (t, t));
                if (s[1].toUpper() != s[2].toUpper()) { EXPECT_FALSE(comparator.operator() < T > (s[1], s[2])); }
                else { EXPECT_TRUE(comparator.operator() < T > (s[1], s[2])); }
            };

            constexpr Type types[] = { Type::QByteArray, Type::QString, };
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

TEST(FileSystemAccessor, Read) {
    using fsa = WritingMaterialsManager::FileSystemAccessor;

    // create test files
    std::filesystem::create_directory("test");
    std::filesystem::create_directory("test/FileSystemAccessor");
    const std::string pwd = std::filesystem::absolute(std::filesystem::path("test/FileSystemAccessor")).string();

    constexpr size_t N = 10000;                     // number of test files
    constexpr size_t Lu = 16;                       // u means unit
    constexpr size_t Lmin = Lu, Lmax = 8ull << 20;  // min/max length of test files

    std::set<uintmax_t> basenames;              // basenames of test files
    std::unordered_set<QByteArray> contents;    // contents of test files

    std::mt19937_64& R = tiny_random::random_engine;
    std::exponential_distribution E(0.001);
    for (size_t i = 0; i < N; ++i) { // generate N regular files
        const uintmax_t basename = tiny_random::number::integer();
        std::ofstream f(pwd + '/' + std::to_string(basename) + ".txt", std::ios::out | std::ios::trunc | std::ios::binary);
        basenames.emplace(basename);
        const size_t L = std::min(Lmax, std::max(Lmin, static_cast<size_t>(Lu * E(R)))); // most files are small and a few of them are big.
        QByteArray content;
        for (size_t j = 0; j < L; j += sizeof(uintmax_t)) { // fill the content in random for this test file
            const uintmax_t n[2] = { tiny_random::number::integer(), 0 }; // with null terminator
            const char* const raw = reinterpret_cast<const char*>(n);
            content.append(QByteArray::fromRawData(raw, sizeof(uintmax_t)));
        }
        f.write(content.constData(), content.size());
        contents.emplace(content);
    }

    // interface test: read
    for (const auto& basename: basenames) { // regular files
        const std::shared_ptr<QFile> f = fsa::Open(QString::fromLocal8Bit(pwd + '/' + std::to_string(basename) + ".txt"));
        const std::shared_ptr<QFileInfo> fi = fsa::GetFileInfo(f);      // read file info
        EXPECT_TRUE(basenames.contains(fi->baseName().toULongLong()));
        EXPECT_TRUE(contents.contains(fsa::GetAllRawContents(f)));      // read file content
    }
    for (size_t i = 0; i < N / 2; ++i) { // open exception test
        bool has_open_exception = false;
        try { fsa::Open(QString::fromLocal8Bit(pwd + "/open-exception-test" + tiny_random::chr::ASCII_string(32) + ".sys")); } // deliberately open a non-existing file
        catch (const std::runtime_error& e) {
            has_open_exception = true;
            EXPECT_EQ(std::string(e.what()).substr(0, 10), std::string("Open file "));
        }
        EXPECT_TRUE(has_open_exception);
    }
}

TEST(JSONFormatter, Default) {
    using fsa = WritingMaterialsManager::FileSystemAccessor;

    //GTEST_SKIP();
    // create test files
    std::filesystem::create_directory("test");
    std::filesystem::create_directory("test/JSONFormatter");
    const std::string pwd = std::filesystem::absolute(std::filesystem::path("test/JSONFormatter")).string();

    constexpr size_t N = 500; // number of test files
    
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

    //GTEST_SKIP();
    WritingMaterialsManager::JSONFormatter formatter;
    for (const auto& basename : basenames) { // format
        const auto f = fsa::Open(QString::fromStdString(pwd) + '/' + std::to_string(basename).c_str() + ".json", QIODevice::ReadOnly);
        auto content = fsa::GetAllRawContents(f);
        auto json = QString::fromUtf8(content);
        formatter.Format(json);
        const auto g = fsa::Open(QString::fromStdString(pwd) + '/' + std::to_string(basename).c_str() + "F.json", QIODevice::WriteOnly);
        g->write(json.toStdString().c_str());
        const auto h = fsa::Open(QString::fromStdString(pwd) + '/' + std::to_string(basename).c_str() + "F.json", QIODevice::ReadOnly);
        EXPECT_EQ(content, fsa::GetAllRawContents(h));
    }
    for (size_t i = 0; i < N / 2; ++i) { // exception test
        auto json = QString::fromUtf8(tiny_random::chr::JSON() + R"(,,/,{,.，，。}:::+_~!@#$%^&*(),,\,<<>><<<>>>??**?*?*?*)"); // construct illegal JSON
        bool has_open_exception = false;
        try { formatter.Format(json); }
        catch (const std::runtime_error& e) {
            has_open_exception = true;
            const QByteArray exception_info = QByteArray::fromRawData(e.what(), strlen(e.what()));
            QRegularExpression re(R"(Exception\s+at\s+.*JSONFormatter.*Format.*: Parsing ERROR)");
            EXPECT_TRUE(re.match(exception_info).hasMatch());
        }
    }
}

TEST(MongoDBAccessor, BasicInfo) {
    WritingMaterialsManager::MongoDBAccessor mongoa;
    QJsonParseError e;
    { // database information
        const auto db_inf = mongoa.GetDatabasesInformation();
        const auto json = QJsonDocument::fromJson(db_inf, &e);
        EXPECT_EQ(e.error, QJsonParseError::ParseError::NoError);   // prove that the returned JSON exactly has NO lexical/syntax error
        const auto root = json.array();
        for (const auto& subnode : root) {
            const QJsonObject unwrapped_subnode = subnode.toObject();
            constexpr const char* const keys[] = { "name", "sizeOnDisk", "empty", };
            for (const auto key : keys) { EXPECT_TRUE(unwrapped_subnode.contains(key)); }
        }
    }
    { // collection information
        constexpr const char* const db_names[] = { "admin", "config", "local", };
        for (const auto db_name : db_names) {
            const auto coll_inf = mongoa.GetCollectionsInformation(db_name);
            const auto json = QJsonDocument::fromJson(coll_inf, &e);
            EXPECT_EQ(e.error, QJsonParseError::ParseError::NoError);   // prove that the returned JSON exactly has NO lexical/syntax error
            const auto root = json.array();
            for (const auto& subnode : root) {
                const QJsonObject unwrapped_subnode = subnode.toObject();
                constexpr const char* const keys[] = { "name", "type", "options", "info", "idIndex", };
                for (const auto key : keys) { EXPECT_TRUE(unwrapped_subnode.contains(key)); }
            }
        }
    }
    { // information of database & collection
        const auto general_inf = mongoa.GetDBsAndCollsInfo();
        const auto json = QJsonDocument::fromJson(general_inf, &e);
        EXPECT_EQ(e.error, QJsonParseError::ParseError::NoError);   // prove that the returned JSON exactly has NO lexical/syntax error
        const auto root = json.array();
        for (const auto& subnode : root) {
            const QJsonObject unwrapped_subnode = subnode.toObject();
            constexpr const char* const keys[] = { "name", "sizeOnDisk", "empty", "Collections", };
            for (const auto key : keys) { EXPECT_TRUE(unwrapped_subnode.contains(key)); }
        }
    }
}
