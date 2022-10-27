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

        enum class JSON_data_type { Invalid, Null, Boolean, String, Signed, Unsigned, Double, Array, Object, };
        
        constexpr size_t n = 1; // test count

        wmm::QtTreeModel tree_model;

        auto get_JSON_data_type = [&](const QModelIndex& index) noexcept {
            using enum JSON_data_type;

            const QVariant data = tree_model.data(index);
            switch (data.typeId()) {
            case QMetaType::Nullptr: return Null;
            case QMetaType::Bool: return Boolean;
            case QMetaType::LongLong: return Signed;
            case QMetaType::ULongLong: return Unsigned;
            case QMetaType::Double: return Double;
            case QMetaType::QString: {
                if (index.column() == 0) { return String; }
                else if (index.column() == 1) {
                    if (tree_model.rowCount(index) == 0) { return String; }
                    else { // tree_model.rowCount(index) > 0
                        if (data.value<QString>() == "<Array>") { return Array; }
                        else if (data.value<QString>() == "<Object>") { return Object; }
                    }
                }
            } break;
            }
            return Invalid;
        };
        auto escape = [&](QByteArray& str) noexcept {
            str.replace(R"(\)", R"(\\)");
            str.replace(R"(")", R"(\")");
            str.replace("\f", R"(\f)");
            str.replace("\n", R"(\n)");
            str.replace("\r", R"(\r)");
            str.replace("\t", R"(\t)");
        };

        for (size_t i = 0; i < n; ++i) {
            // generate JSON and import into the tree model
            const auto test_JSON = tiny_random::chr::JSON();
            tree_model.FromJSON(QByteArray::fromStdString(test_JSON));

            // verify
            QByteArray generated_JSON;
            std::stack<QVariant, std::vector<QVariant>> s;
            s.emplace(tree_model.index(0, 1, {}));
            while (s.empty() == false) {
                const QVariant top_node = s.top();
                s.pop();
                switch (top_node.typeId()) {
                case QMetaType::QByteArray:
                    generated_JSON.append(top_node.value<QByteArray>());
                    break;
                case QMetaType::QModelIndex: {
                    using enum JSON_data_type;
                    
                    const auto index = top_node.value<QModelIndex>();
                    const QVariant value = tree_model.data(index);
                    const auto data_type = get_JSON_data_type(index);
                    QVERIFY(data_type != Invalid);
                    switch (data_type) {
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
                    case String: {
                        generated_JSON.append('\"');
                        QByteArray string = value.value<QString>().toUtf8();
                        escape(string);
                        generated_JSON.append(string);
                        generated_JSON.append('\"');
                    } break;
                    case Signed:
                        generated_JSON.append(QByteArray::number(value.value<int64_t>()));
                        break;
                    case Unsigned:
                        generated_JSON.append(QByteArray::number(value.value<uint64_t>()));
                        break;
                    case Double:
                        generated_JSON.append(QByteArray::number(value.value<double>()));
                        break;
                    case Array: {
                        const auto child_count = tree_model.rowCount(index);
                        s.emplace(QByteArray("]"));
                        for (auto i = child_count - 1; i > 0; --i) {
                            s.emplace(tree_model.index(i, 1, index));
                            s.emplace(QByteArray(","));
                        }
                        s.emplace(tree_model.index(0, 1, index));
                        s.emplace(QByteArray("["));
                    } break;
                    case Object: {
                        const auto child_count = tree_model.rowCount(index);
                        s.emplace(QByteArray("}"));
                        for (auto i = child_count - 1; i > 0; --i) {
                            s.emplace(tree_model.index(i, 1, index));
                            s.emplace(QByteArray(":"));
                            s.emplace(tree_model.index(i, 0, index));
                            s.emplace(QByteArray(","));
                        }
                        s.emplace(tree_model.index(0, 1, index));
                        s.emplace(QByteArray(":"));
                        s.emplace(tree_model.index(0, 0, index));
                        s.emplace(QByteArray("{"));
                    } break;
                    }
                } break;
                }
            }
            QJsonParseError e[2];
            const QJsonDocument d[2] = { 
                QJsonDocument::fromJson(QByteArray::fromStdString(test_JSON), e + 0),
                QJsonDocument::fromJson(generated_JSON, e + 1),
            };
            {
                QDir wd("test/QtTreeModel");
                if (wd.exists() == false) { QDir::current().mkdir(wd.path()); }
                QFile f[2] = { QFile(wd.path() + "/d0.json"), QFile(wd.path() + "/d1.json") };
                for (size_t i = 0; i < 2; ++i) { f[i].open(QIODevice::OpenModeFlag::WriteOnly); }
                f[0].write(test_JSON.c_str()); f[1].write(generated_JSON);
            }
            QCOMPARE(d[0], d[1]);
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
