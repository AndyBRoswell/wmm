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

auto get_high_resolution_tick_count() { return std::chrono::high_resolution_clock::now().time_since_epoch().count(); }

TEST(TreeEditor, ConvertCharsetForArtificialJSON) {
    constexpr const char* const target_charsets[] = { "big5-hkscs", "gb18030", "utf-16", "utf-16be", "utf-16le", "utf-32", "utf-32be", "utf-32le" };
    std::unordered_map<QByteArray, QTextCodec*> codec; // you should not delete codecs yourself: once created they become Qt's responsibility.
    for (const auto charset : target_charsets) {
        codec.emplace(charset, QTextCodec::codecForName(charset));
    }

    QFile file_list_file("test/TreeEditor/datgen.json");
    if (file_list_file.open(QIODevice::OpenModeFlag::ReadOnly) == false) { return; }
    const QJsonArray file_list = QJsonDocument::fromJson(file_list_file.readAll()).array();
    for (const auto& term : file_list) {
        // get source file and open
        const QJsonObject file = term.toObject();
        QFile input_file(file.value("src").toString());
        input_file.open(QIODevice::OpenModeFlag::ReadOnly);

        // read and convert
        const QByteArray content = input_file.readAll();
        const QByteArray target_file_name_prefix = "a-" + QByteArray::number(get_high_resolution_tick_count());
        for (const auto& c : codec) {
            const QByteArray encoded_string = c.second->fromUnicode(content); // assume utf8
            QFile target_file(target_file_name_prefix + "-" + c.first + ".json");
            target_file.open(QIODevice::OpenModeFlag::WriteOnly);
            target_file.write(encoded_string);
        }
    }
}
