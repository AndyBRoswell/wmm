// std
#include <unordered_map>

// Qt
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QObject>
#include <QSignalSpy>
#include <QString>
#include <QTest>
#include <QTextCodec>

// files to be tested
#include "src/TreeEditor.h"

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 2");
    }

    void TreeEditor__open_JSON() {
        namespace wmm = WritingMaterialsManager;

        wmm::TreeEditor tree_editor;

        qRegisterMetaType<wmm::TreeEditor>();
        std::unordered_map<QByteArray, QSignalSpy> signal_spy;
        signal_spy.emplace("ShouldUpdatePathName()", QSignalSpy(&tree_editor, SIGNAL(ShouldUpdatePathName())));
        signal_spy.emplace("ShouldUpdateFileType()", QSignalSpy(&tree_editor, SIGNAL(ShouldUpdateFileType())));
        signal_spy.emplace("ShouldUpdateCharset()", QSignalSpy(&tree_editor, SIGNAL(ShouldUpdateCharset())));

        const QDir wd("test/TreeEditor");
        const QStringList test_file_list = wd.entryList({ "*.json" });

        for (const auto& test_file_name : test_file_list) {
            const QStringList file_name_part = test_file_name.split("_");
            const QByteArray charset = file_name_part.size() == 3 ? file_name_part[2].toUtf8().toUpper() : "UTF-8";
            //const QByteArray charset = file_name_part.size() == 3 ? file_name_part[2].toUtf8(): "utf-8";

            // test side
            tree_editor.SetCharset(charset);
            QCOMPARE(signal_spy["ShouldUpdateCharset()"].count(), 1);
            tree_editor.OpenFile(test_file_name);
            QCOMPARE(signal_spy["ShouldUpdatePathName()"].count(), 1);
            QCOMPARE(signal_spy["ShouldUpdateFileType()"].count(), 1);

            // verification side
            QFile test_file(test_file_name);
            test_file.open(QIODevice::OpenModeFlag::ReadOnly);
            QJsonParseError JSON_error;
            QTextCodec* codec = QTextCodec::codecForName(charset);
            QTextDecoder* const decoder = codec->makeDecoder();
            const QJsonDocument doc = QJsonDocument::fromJson(decoder->toUnicode(test_file.readAll()).toUtf8(), &JSON_error);
            if (JSON_error.error == QJsonParseError::NoError) {
                
            }
            delete decoder;
        }
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 2");
    }
};

QTEST_MAIN(test)
#include "test.moc"
