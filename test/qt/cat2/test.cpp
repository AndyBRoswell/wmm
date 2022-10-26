// std
#include <unordered_map>

// Qt
#include <QAbstractItemModelTester>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QObject>
#include <QSignalSpy>
#include <QString>
#include <QTest>
#include <QTextCodec>

// this software
#include "tiny_random.h"

// files to be tested
#include "src/TreeEditor.h"

class test : public QObject {
    Q_OBJECT
private:

private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 2");
    }

    void QtTreeModel__construct_from_JSON() {
        namespace wmm = WritingMaterialsManager;

        enum class JSON_data_type { Null, Boolean, String, Signed, Unsigned, Double, Array, Object, };
        
        constexpr size_t n = 10; // test count

        wmm::QtTreeModel tree_model;

        auto get_JSON_data_type = [&](const QModelIndex& index) noexcept {
            using enum JSON_data_type;

            const QVariant data = tree_model.data(index);
            switch (tree_model.rowCount(index)) {
            case 0:
                switch (data.typeId()) {
                case QMetaType::Nullptr: return Null;
                case QMetaType::Bool: return Boolean;
                case QMetaType::QString: return String;
                case QMetaType::LongLong: return Signed;
                case QMetaType::ULongLong: return Unsigned;
                case QMetaType::Double: return Double;
                }
            default:
                if (data.value<QString>() == "<Array>") { return Array; }
                else { return Object; }
            }
        };

        for (size_t i = 0; i < n; ++i) {
            // generate JSON and import into the tree model
            tree_model.FromJSON(QByteArray::fromStdString(tiny_random::chr::JSON()));

            // verify
            QByteArray generated_JSON;
            std::stack<QVariant, std::vector<QVariant>> s;
            s.emplace(tree_model.index(0, 1, {}));
            while (s.empty() == false) {
                const QVariant n = s.top();
                s.pop();
                switch (n.typeId()) {
                case QMetaType::QByteArray:
                    generated_JSON.append(n.value<QByteArray>());
                    break;
                case QMetaType::QModelIndex: {
                    using enum JSON_data_type;

                    const QVariant value = tree_model.data(n.value<QModelIndex>());
                    switch (static_cast<JSON_data_type>(value.typeId())) {
                    case Null:
                        generated_JSON.append("null");
                        break;
                    case Boolean:
                        switch (value.value<bool>()) {
                        case false:
                            generated_JSON.append("false");
                            break;
                        case true:
                            generated_JSON.append("true");
                            break;
                        }
                        break;
                    case Signed:
                        break;
                    }
                } break;
                }
            }
        }
    }

    void TreeEditor__open_JSON() {
        namespace wmm = WritingMaterialsManager;

        wmm::TreeEditor tree_editor;

        qRegisterMetaType<wmm::TreeEditor>();
        QSignalSpy signal_spy_ShouldUpdatePathName(&tree_editor, SIGNAL(ShouldUpdatePathName()));
        QSignalSpy signal_spy_ShouldUpdateFileType(&tree_editor, SIGNAL(ShouldUpdateFileType()));
        QSignalSpy signal_spy_ShouldUpdateCharset(&tree_editor, SIGNAL(ShouldUpdateCharset()));

        const QDir wd("test/TreeEditor");
        const QStringList test_file_list = wd.entryList({ "*.json" });

        for (const auto& test_file_name : test_file_list) {
            const QStringList file_name_part = test_file_name.split("_");
            const QByteArray charset = file_name_part.size() == 3 ? file_name_part[2].toUtf8().toUpper() : QByteArray("UTF-8");
            //const QByteArray charset = file_name_part.size() == 3 ? file_name_part[2].toUtf8(): "utf-8";

            // test side
            tree_editor.SetCharset(charset);
            QCOMPARE(signal_spy_ShouldUpdateCharset.count(), 1);
            tree_editor.OpenFile(test_file_name);
            QCOMPARE(signal_spy_ShouldUpdatePathName.count(), 1);
            QCOMPARE(signal_spy_ShouldUpdateFileType.count(), 1);

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
