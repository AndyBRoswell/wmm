// std
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

TEST(TreeEditor, ConvertCharsetForArtificialJSON) {
    constexpr const char* const target_charsets[] = { "big5-hkscs", "gb18030", "utf-8", "utf-16", "utf-16be", "utf-16le", "utf-32", "utf-32be", "utf-32le" };
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

    }
}
