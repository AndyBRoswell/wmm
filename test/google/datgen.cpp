// std
#include <chrono>
#include <unordered_map>

// Qt
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextCodec>

// googletest
#include <gtest/gtest.h>

const QByteArray default_delim = "_";

auto get_high_resolution_tick_count() { return std::chrono::high_resolution_clock::now().time_since_epoch().count(); }

TEST(TreeEditor, ConvertCharsetForArtificialJSON) {
    GTEST_SKIP();
    constexpr const char* const target_charsets[] = { "gb18030", "utf-16", "utf-16be", "utf-16le", /*"utf-32", "utf-32be", "utf-32le",*/ };
    std::unordered_map<QByteArray, QTextCodec*> codec; // you should not delete codecs yourself: once created they become Qt's responsibility.
    for (const auto charset : target_charsets) {
        codec.emplace(charset, QTextCodec::codecForName(charset));
    }

    const QByteArray wd = QByteArray("test/TreeEditor");
    QFile file_list_file(wd + "/datgen.json");
    if (file_list_file.open(QIODevice::OpenModeFlag::ReadOnly) == false) { return; }
    const QJsonArray file_list = QJsonDocument::fromJson(file_list_file.readAll()).array();
    for (const auto& term : file_list) {
        // get source file and open
        const QJsonObject file = term.toObject();
        QFile source_file(file.value("src").toString());
        source_file.open(QIODevice::OpenModeFlag::ReadOnly);

        // read and convert
        const QByteArray content = source_file.readAll();
        const QByteArray target_file_name_prefix = "a" + default_delim + QByteArray::number(get_high_resolution_tick_count());
        { // also make a copy of the original file
            QFile target_file(wd + "/" + target_file_name_prefix + ".json");
            target_file.open(QIODevice::OpenModeFlag::WriteOnly);
            target_file.write(content);
        }
        for (const auto& c : codec) {
            const QByteArray encoded_content = c.second->fromUnicode(content); // assume utf8
            QFile target_file(wd + "/" + target_file_name_prefix + default_delim + c.first + ".json");
            target_file.open(QIODevice::OpenModeFlag::WriteOnly);
            target_file.write(encoded_content);
        }
    }
}
