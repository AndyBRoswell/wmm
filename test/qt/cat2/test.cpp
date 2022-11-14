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

// rapidjson
#include "rapidjson/document.h"

// this software
#include "tiny_random.h"
#include "util.h"

// files to be tested
#include "src/TreeEditor.h"
#include "src/TreeView.h"

class test : public QObject {
    Q_OBJECT
private:
    static constexpr Qt::MouseButton mouse_keys[] = { Qt::MouseButton::LeftButton, Qt::MouseButton::RightButton, Qt::MouseButton::MiddleButton };

    bool QtTreeModel_test(QAbstractItemModel& tree_model, const std::string& test_JSON) {
        enum class JSON_data_type { Invalid, Null, Boolean, String, Signed, Unsigned, Double, Array, Object, };

        auto get_JSON_data_type = [&](const QModelIndex& index) noexcept {
            using enum JSON_data_type;

            const QVariant data = tree_model.data(index);
            switch (data.typeId()) {
            case QMetaType::Nullptr: return Null;
            case QMetaType::Bool: return Boolean;
            case QMetaType::LongLong: return Signed;
            case QMetaType::ULongLong: return Unsigned;
            case QMetaType::Double: return Double;
            case QMetaType::QString: return String;
            case QMetaType::QByteArray:
                if (data.value<QByteArray>() == "<Array>") { return Array; }
                else if (data.value<QByteArray>() == "<Object>") { return Object; }
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

        // generate JSON from tree model
        QByteArray generated_JSON;
        std::stack<QVariant, std::vector<QVariant>> s;
        s.emplace(tree_model.index(0, 1, {}));
        while (s.empty() == false) {
            const QVariant top_node = s.top();
            s.pop();
            switch (top_node.typeId()) {
            case QMetaType::QByteArray: generated_JSON.append(top_node.value<QByteArray>()); break;
            case QMetaType::QModelIndex: {
                using enum JSON_data_type;

                const auto index = top_node.value<QModelIndex>();
                const QVariant value = tree_model.data(index);
                const auto data_type = get_JSON_data_type(index);
                if (data_type == Invalid) { throw std::runtime_error("Invalid JSON data type"); }
                switch (data_type) {
                case Null: generated_JSON.append("null"); break;
                case Boolean:
                    switch (value.value<bool>()) {
                    case false: generated_JSON.append("false"); break;
                    case true: generated_JSON.append("true"); break;
                    }
                    break;
                case String: {
                    generated_JSON.append('\"');
                    QByteArray string = value.value<QString>().toUtf8();
                    escape(string);
                    generated_JSON.append(string);
                    generated_JSON.append('\"');
                } break;
                case Signed: generated_JSON.append(QByteArray::number(value.value<int64_t>())); break;
                case Unsigned: generated_JSON.append(QByteArray::number(value.value<uint64_t>())); break;
                case Double: generated_JSON.append(QByteArray::number(value.value<double>(), 'g', DBL_DECIMAL_DIG)); break;
                case Array: {
                    const auto child_count = tree_model.rowCount(index);
                    if (child_count == 0) { s.emplace(QByteArray("[]")); }
                    else {
                        s.emplace(QByteArray("]"));
                        for (auto i = child_count - 1; i > 0; --i) {
                            s.emplace(tree_model.index(i, 1, index));
                            s.emplace(QByteArray(","));
                        }
                        s.emplace(tree_model.index(0, 1, index));
                        s.emplace(QByteArray("["));
                    }
                } break;
                case Object: {
                    const auto child_count = tree_model.rowCount(index);
                    if (child_count == 0) { s.emplace(QByteArray("{}")); }
                    else {
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
                    }
                } break;
                }
            } break;
            }
        }

        // verify
        QJsonParseError e[2];
        const QJsonDocument d[2] = {
            QJsonDocument::fromJson(QByteArray::fromStdString(test_JSON), e + 0),
            QJsonDocument::fromJson(generated_JSON, e + 1),
        };
        if (e[0].error != QJsonParseError::ParseError::NoError) { throw std::runtime_error("Syntax ERROR in reference JSON"); }
        if (e[1].error != QJsonParseError::ParseError::NoError) { throw std::runtime_error("Syntax ERROR in generated JSON"); };
        if (d[0] != d[1]) {
            QDir wd("test/QtTreeModel");
            if (wd.exists() == false) { QDir::current().mkdir(wd.path()); }
            QFile f[2] = { QFile(wd.path() + "/d0.json"), QFile(wd.path() + "/d1.json") };
            for (size_t i = 0; i < 2; ++i) { f[i].open(QIODevice::OpenModeFlag::WriteOnly); }
            f[0].write(d[0].toJson()); f[1].write(d[1].toJson());
            return false;
        }
        return true;
    }

private slots:
    void initTestCase() {
        qDebug("GUI Test Cat. 2");
    }

    void TreeView__basic() {
        namespace wmm = WritingMaterialsManager;

        constexpr size_t n = 1e6; // test count

        wmm::TreeView tree_view;

        // signals
        qRegisterMetaType<wmm::TreeView>();
        QSignalSpy spy(&tree_view, SIGNAL(MouseDown()));
        util::disable_test_info();
        for (size_t i = 0; i < n; ++i) {
            for (const auto m : mouse_keys) {
                QTest::mouseClick(tree_view.viewport(), m);
                QTest::mouseDClick(tree_view.viewport(), m);
                QTest::mousePress(tree_view.viewport(), m);
                QTest::mouseRelease(tree_view.viewport(), m);
            }
        }
        util::enable_test_info();
        QCOMPARE(spy.count(), 6 * n); // click, press
    }

    void QtTreeModel__construct_from_JSON() {
        namespace wmm = WritingMaterialsManager;

        constexpr size_t n = 500; // test count

        wmm::QtTreeModel tree_model;

        util::disable_test_info();
        for (size_t i = 0; i < n; ++i) {
            qDebug("[" + QByteArray::number(i + 1) + " / " + QByteArray::number(n) + "]");
            qDebug("Generating reference JSON ...");
            const auto test_JSON = tiny_random::chr::JSON();
            qDebug("Reference JSON generated.");
            qDebug("Constructing tree structure from reference JSON ...");
            tree_model.FromJSON(QByteArray::fromStdString(test_JSON)); // import JSON
            qDebug("Tree structure constructed.");
            qDebug("Verifying the equivalence of these 2 tree structures ...");
            QVERIFY(QtTreeModel_test(tree_model, test_JSON));
            qDebug("Congratulations: Reference JSON and generated JSON are equivalent, the tree model worked correctly.");
        }
        util::enable_test_info();
    }

    void TreeEditor__open_JSON() {
        namespace wmm = WritingMaterialsManager;

        wmm::TreeEditor tree_editor;
        { // exception test
            constexpr size_t n = 1e3; // test count

            for (size_t i = 0; i < n; ++i) {
                tree_editor.SetFileType("JSON");
                QCOMPARE(tree_editor.GetFileType(), "JSON");
                const auto invalid_file_type = QByteArray::fromStdString(tiny_random::chr::ASCII_string(1000));
                tree_editor.SetFileType(invalid_file_type);
                QCOMPARE(tree_editor.GetFileType(), "/* File Type Not Supported */");

                const auto invalid_JSON = QString::fromStdString(tiny_random::chr::ASCII_string(tiny_random::number::integer(1ull, 1000ull)) + R"(,,<><<>><<<>>>\[,:.:]{:_:}[}{]..)");
                tree_editor.SetText(invalid_JSON);
                QCOMPARE(tree_editor.GetText(), invalid_JSON);
            }
        }
        { // TreeEditor::FromJSON
            qRegisterMetaType<wmm::TreeEditor>();
            QSignalSpy signal_spy_ShouldUpdatePathName(&tree_editor, SIGNAL(ShouldUpdatePathName()));
            QSignalSpy signal_spy_ShouldUpdateFileType(&tree_editor, SIGNAL(ShouldUpdateFileType()));
            QSignalSpy signal_spy_ShouldUpdateCharset(&tree_editor, SIGNAL(ShouldUpdateCharset()));

            const QDir wd("test/TreeEditor");
            const QStringList test_file_entry = wd.entryList({ "*.json" });

            for (auto i = 0; i < test_file_entry.size(); ++i) {
                const auto& file_name = test_file_entry[i];
                qDebug("[" + QByteArray::number(i) + "] " + "testing " + file_name.toUtf8());
                auto suffix_pos = file_name.lastIndexOf('.');
                if (suffix_pos == -1) { suffix_pos = file_name.size(); }
                const QStringList base_name_part = file_name.first(suffix_pos).split('_');
                const QByteArray charset = base_name_part.size() == 3 ? base_name_part[2].toUtf8().toUpper() : QByteArray("UTF-8");

                // test side
                tree_editor.SetCharset(charset);
                QCOMPARE(signal_spy_ShouldUpdateCharset.count(), 1);
                const auto path_name = wd.path() + "/" + file_name;
                tree_editor.OpenFile(path_name);
                QCOMPARE(signal_spy_ShouldUpdatePathName.count(), 1);
                QCOMPARE(signal_spy_ShouldUpdateFileType.count(), 1);

                // verification side
                QFile test_file(path_name);
                test_file.open(QIODevice::OpenModeFlag::ReadOnly);
                QJsonParseError JSON_error;
                QTextCodec* text_codec = QTextCodec::codecForName(charset);
                QTextDecoder* const text_decoder = text_codec->makeDecoder();
                const auto test_JSON = text_decoder->toUnicode(test_file.readAll()).toUtf8();
                const QJsonDocument doc = QJsonDocument::fromJson(test_JSON, &JSON_error);
                delete text_decoder;
                if (JSON_error.error == QJsonParseError::NoError) {
                    try { QVERIFY(QtTreeModel_test(*tree_editor.IntuitiveView->model(), test_JSON.toStdString())); }
                    catch (const std::exception& e) { qFatal(e.what()); }
                }

                signal_spy_ShouldUpdatePathName.clear();
                signal_spy_ShouldUpdateFileType.clear();
                signal_spy_ShouldUpdateCharset.clear();
            }
        }
    }

    void cleanupTestCase() {
        qDebug("End GUI Test Cat. 2");
    }
};

QTEST_MAIN(test)
#include "test.moc"
