#include "Test.h"

// standard libraries
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <numbers>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <vector>

// Qt libs
#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QProcess>
#include <QString>
#include <QStringView> // QStringRef removed in Qt 6.
#include <QTextCodec>
#include <QTextStream>

// mongocxx
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

// json libs
//#include "simdjson.h"
#include "rapidjson/document.h"

// DuckX
#include <duckx.hpp>

// custom includes
#include "global.h"

#include "QFileSystemModelDialog.h"

#include "MongoDBAccessor.h"

using WMMTest = WritingMaterialsManager::Test;

std::string WMMTest::LastStartedFn;
std::string WMMTest::LastFinishedFn;

int WMMTest::Start() {
    qDebug() << "Start running test ...";

    int ret;

    std::vector<std::function<void(void)>> TestFunctions{
        //SimpleTest,
        //Qt::EncodingOfFileRW,
        //Qt::Quick::TextAreaKeyEvent,
        //simdjson::Demo,
        //RapidJSON::Demo,
        //mongocxx::Tutorial,
        mongocxx::CustomDataDemo,
        //DuckX::QuickStart,
        //Qt::Widgets::Demo,
        //Qt::InterProcessCommunication,
        //Qt::GetSystemLocale,
    };

    for (const std::function<void(void)>& f : TestFunctions) {
        f();
        qDebug() << Sep << LastFinishedFn.c_str() << "completed." << Sep;
    }

    qDebug() << "All tests completed!";
    return EXIT_SUCCESS;
}

int WMMTest::SimpleTest() {
    LastStartedFn.assign(__FUNCTION__);

    // Verify C++20 is supported
    const std::filesystem::path p = R"(C:\)";
    const auto pi = std::numbers::pi;

    LastFinishedFn.assign(__FUNCTION__);
    return EXIT_SUCCESS;
}

void WMMTest::Qt::EncodingOfFileRW() {
    using std::move;

    LastStartedFn.assign(__FUNCTION__);

    try {
        QFile UTF8ReadTestFile("test/test-data/utf8test.json"); // don't input wrong file name
        if (UTF8ReadTestFile.open(QIODevice::ReadOnly) == false) {
            throw std::runtime_error("Exception when opening test/testdata/utf8test.json in read-only mode.");
        }
        QByteArray UTF8TestString = move(UTF8ReadTestFile.readAll());
        qDebug() << "UTF8 Test Doc:";
        qDebug() << QString::fromUtf8(UTF8TestString);
        QFile UTF8WriteTestFile("test/test-data/utf8testw.json"); // by default cwd is the build dir
        if (UTF8WriteTestFile.open(QIODevice::WriteOnly) == false) {
            throw std::runtime_error("Exception when opening test/testdata/utf8testw.json in write-only mode.");
        }
        UTF8WriteTestFile.write(UTF8TestString);
    }
    catch (const std::runtime_error& e) {
        qDebug() << e.what();
    }

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::Qt::GetSystemLocale() {
    LastStartedFn.assign(__FUNCTION__);

    qDebug() << QLocale::system();
    qDebug() << QTextCodec::codecForLocale()->aliases();

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::Qt::InterProcessCommunication() {
    LastStartedFn.assign(__FUNCTION__);

    qDebug() << "Current operating system:";
#ifdef _WIN64
    qDebug() << "Windows 64-bit";
#elif __unix || __unix__
    qDebug() << "Unix";
#elif __linux__
    qDebug() << "Linux";
#elif __APPLE__ || __MACH__
    qDebug() << "Mac OS X";
#else
    qDebug() << "Unknown operating system.";
#endif

    using namespace std;
    using namespace std::chrono;
    using namespace std::chrono_literals;

    const QStringList TestProcessName{ "python"/*, "mongosh"*/ };
    const QList<QStringList> TestProcessArguments{ { "-c", "import this" },
                                                  {} };
    //    const QList<QStringList> TestProcessArguments{{},
    //                                                  {}};
    const QList<int> QProcessWaitingTimeout{ 30 * 1000, 1 * 1000 };
    const QList<decltype(1s)> OutputWaitingDuration{ 120s, 5s };

    for (qsizetype i = 0; i < TestProcessName.size(); ++i) {
        const shared_ptr<QProcess> Process(new QProcess);
        Process->start(TestProcessName[i], TestProcessArguments[i]);
        qDebug() << "Starting test process" << Process->program() << ", timeout:" << QProcessWaitingTimeout[i] << "ms ...";
        qDebug() << (Process->waitForStarted(QProcessWaitingTimeout[i]) ? "Succeeded." : "Failed.");
        const time_point<high_resolution_clock> StartTimePoint = high_resolution_clock::now();
        qDebug() << "Demonstrating output ...";
        while (high_resolution_clock::now() - StartTimePoint <= OutputWaitingDuration[i]) {
            Process->waitForReadyRead(QProcessWaitingTimeout[i]);
            qDebug() << Process->readAllStandardOutput();
        }
        qDebug() << "Output demonstration completed.";
        qDebug() << "Waiting for the completion of" << Process->program() << ", timeout:" << QProcessWaitingTimeout[i] << "ms ...";
        qDebug() << (Process->waitForFinished(QProcessWaitingTimeout[i]) ? "Succeeded." : "Failed.");
    }

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::Qt::Widgets::Demo() {
    LastStartedFn.assign(__FUNCTION__);

    auto* qFileSystemDialog = new QFileSystemModelDialog();
    qFileSystemDialog->showMaximized();

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::Qt::Quick::TextAreaKeyEvent() {
    LastStartedFn.assign(__FUNCTION__);

    // preparation
    QList<QObject*> TextEditors = MainWindow->findChildren<QObject*>("TextEditor");
    const QByteArray DemoJSONString("{\n\"text\": \"Hello JSON\"\n}");
    const QJsonDocument DemoJSONDocument = QJsonDocument::fromJson(DemoJSONString);
    const QByteArray IndentedDemoJSONString = DemoJSONDocument.toJson(); // QJsonDocument::Indented as default arg
    if (TextEditors.empty() == false) {
        for (auto TextEditor : TextEditors) {
            TextEditor->setProperty("text", IndentedDemoJSONString);
        }
    }

    // set the focus on the text editor of search result
    auto* const tabMain = MainWindow->findChild<QObject*>("MainTabBar");
    tabMain->setProperty("currentIndex", 1);
    QObject* const ResultEditor = TextEditors[1];
    ResultEditor->setProperty("focus", true);

    // simulate key press
    // Qt doesn't support "KeyDown" event since it is platform-dependent.
    const auto KeySeq = { ::Qt::Key_Down, ::Qt::Key_Down, ::Qt::Key_Enter, ::Qt::Key_Up, ::Qt::Key_End, };
    for (auto Key : KeySeq) { QGuiApplication::postEvent(ResultEditor, new QKeyEvent(QEvent::KeyPress, Key, ::Qt::NoModifier)); }
    // If key is 0, the event is not a result of a known key; for example, it may be the result of a compose sequence or keyboard macro.
    QString ExtraString = R"(,"text2":"Hello Qt Quick",  "text3":"rtsp","text3":"寄了","text4":"炸穿地心","flag":true,"count":1412)";
    QGuiApplication::postEvent(ResultEditor, new QKeyEvent(QEvent::KeyPress, 0, ::Qt::NoModifier, ExtraString));
    QGuiApplication::postEvent(ResultEditor, new QKeyEvent(QEvent::KeyPress, ::Qt::Key_Enter, ::Qt::NoModifier));

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::simdjson::Demo() {
    //using namespace ::simdjson;
    //using std::endl;
    //using std::string;
    //using std::string_view;
    //using std::exception;

    //LastStartedFn.assign(__FUNCTION__);

    //try {
    //    ondemand::parser Parser; // A parser may have at most one document open at a time, since it holds allocated memory used for the parsing.
    //    auto TgChannelContentsInJSON = padded_string::load("test/test-data/Haruhi的沙雕日常.json"); // Tg: telegram
    //    auto TgChannelContents = Parser.iterate(TgChannelContentsInJSON); // position a pointer at the beginning of the JSON data
    //    const string_view ret = TgChannelContents["name"].get_string().value_unsafe();
    //    qDebug() << static_cast<string>(ret).c_str();
    //}
    //catch (const exception& e) {
    //    qDebug() << "Exception thrown at WMMTest::Demo():";
    //    qDebug() << e.what();
    //}

    //LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::RapidJSON::Demo() {
    using std::exception;
    using std::runtime_error;
    using std::move;

    LastStartedFn.assign(__FUNCTION__);

    static const char* TypeName[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

    qDebug() << "CWD: " << std::filesystem::current_path().generic_string().c_str();

    // read json file for this test
    QFile TgChannelJSONFile("test/test-data/Haruhi的沙雕日常.json");
    if (TgChannelJSONFile.open(QIODevice::ReadOnly) == false) {
        throw runtime_error(R"(Open test file "test/test-data/Haruhi的沙雕日常.json" failed.)");
    }
    QTextStream TgChannelJSONFileStream(&TgChannelJSONFile);
    QString TgChannelJSONString = move(TgChannelJSONFileStream.readAll());
    qDebug() << "Contents of the test file:";
    qDebug() << QStringView(TgChannelJSONString).sliced(0, 256);

    // JSON processing test
    rapidjson::Document TgChannelContents;
    QByteArray TgChannelJSONStringUTF8 = move(TgChannelJSONString.toUtf8());
    try {
        if (TgChannelContents.Parse(TgChannelJSONStringUTF8.constData()).HasParseError()) {
            throw runtime_error(R"(JSON Parsing Error)");
        }
    }
    catch (const runtime_error& e) {
        qDebug() << "Exception thrown at" << __FUNCTION__;
        qDebug() << e.what();
    }
    try {
        qDebug() << TgChannelContents["type"].GetString();
        qDebug() << TgChannelContents["name"].GetString();
        qDebug() << TgChannelContents["id"].GetUint64();

        const auto TgChannelMsgs = TgChannelContents["messages"].GetArray();
        const size_t MsgCnt = 4;
        for (rapidjson::Value::ConstValueIterator i = TgChannelMsgs.Begin(), it = i + MsgCnt; i != it; ++i) {
            qDebug() << "Message" << i - TgChannelMsgs.Begin();
            const auto Message = i->GetObject();
            {
                const auto is = Message.MemberBegin();
                qDebug() << '<' << is->name.GetString() << "," << TypeName[is->value.GetType()] << "," << is->value.GetUint64() << '>';
            }
            for (rapidjson::Value::ConstMemberIterator j = Message.MemberBegin() + 1; j != Message.MemberEnd(); ++j) {
                switch (j->value.GetType()) {
                case rapidjson::kStringType: qDebug() << '<' << j->name.GetString() << ',' << "String" << ',' << j->value.GetString() << '>'; break;
                case rapidjson::kObjectType: qDebug() << '<' << j->name.GetString() << ',' << "{Object}" << '>'; break;
                case rapidjson::kArrayType: qDebug() << '<' << j->name.GetString() << ',' << "[Array]" << '>'; break;
                default: qDebug() << '<' << j->name.GetString() << ',' << TypeName[j->value.GetType()] << ',' << "(...)" << '>'; break;
                }
            }
        }
    }
    catch (const exception& e) {
        qDebug() << "Exception thrown at" << __FUNCTION__;
        qDebug() << "JSON object access error.";
    }

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::mongocxx::Tutorial() {
    using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;
    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::open_document;

    LastStartedFn.assign(__FUNCTION__);

    ::mongocxx::uri local_MongoDB_URI("mongodb://localhost:27017/?directConnection=true&serverSelectionTimeoutMS=2000");
    ::mongocxx::client local_client(local_MongoDB_URI);
    ::mongocxx::database test = local_client["test"];
    ::mongocxx::collection coll = test["coll"];

    // create a document
    /*
        {
           "name" : "MongoDB",
           "type" : "database",
           "count" : 1,
           "versions": [ "v3.2", "v3.0", "v2.6" ],
           "info" : {
                       "x" : 203,
                       "y" : 102
                    }
        }
    */
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder // bsoncxx::document::value type is a read-only object owning its own memory. To use it, you must obtain a bsoncxx::document::view using the view() method.
        << "name" << "MongoDB"
        << "type" << "database"
        << "count" << 1
        << "versions" << bsoncxx::builder::stream::open_array
        << "v3.2" << "v3.0" << "v2.6"
        << close_array
        << "info" << bsoncxx::builder::stream::open_document
        << "x" << 203
        << "y" << 102
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize; // Use bsoncxx::builder::stream::finalize to obtain a bsoncxx::document::value instance.
    bsoncxx::document::view view = doc_value.view();
    bsoncxx::document::element element = view["name"];
    if (element.type() != bsoncxx::type::k_utf8) {
        throw std::runtime_error("Exception at WMMTest::Tutorial(): The type of this element of the object is unexpected.");
    }
    std::string_view name = element.get_utf8().value;
    qDebug() << R"("name":")" << name.data() << '\"';

    // insert a document
    bsoncxx::stdx::optional<::mongocxx::result::insert_one> insert_result = coll.insert_one(bsoncxx::document::view_or_value(doc_value));
    qDebug() << "doc id:" << QString::fromUtf8(insert_result->inserted_id().get_oid().value.to_string());

    // drop db
    test.drop();

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::mongocxx::CustomDataDemo() {
    using namespace std;

    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;

    LastStartedFn.assign(__FUNCTION__);

    // prepare db and collection for test
    ::mongocxx::uri LocalURI("mongodb://localhost:27017/?directConnection=true&serverSelectionTimeoutMS=2000");
    const auto Local = ::mongocxx::client(LocalURI);
    ::mongocxx::database test = Local["test"];
    ::mongocxx::collection coll = test["coll"];

    // drop db
    test.drop();

    // prepare JSON strings for test
    vector<shared_ptr<QFile>> TestFiles{
        //        new QFile("test/test-data/utf8test.json"),
        //        new QFile("test/test-data/utf16letest.json"),
        //        new QFile("test/test-data/Haruhi的沙雕日常.json"),
    };
    const QDir TestDataDir("test/data");
    const QStringList ExtraTestFiles = TestDataDir.entryList({ "*.json" });
    for (const auto& ExtraTestFile : ExtraTestFiles) {
        TestFiles.emplace_back(new QFile("test/data/" + ExtraTestFile));
    }
    vector<shared_ptr<QTextStream>> StreamsForTestFiles;
    vector<QByteArray> TestStrings;
    for (const auto& TestFile : TestFiles) {
        TestFile->open(QIODevice::ReadOnly); // don't forget to open file
        TestStrings.emplace_back(move(TestFile->readAll()));
    }
    // insert new JSON docs and list the insertion results
    try {
        for (const auto& TestString : TestStrings) {
            //            qDebug() << "Inserting JSON document:" << QString::fromUtf8(TestString);
            coll.insert_one(bsoncxx::from_json(TestString.data())); // insert a doc (must comply the format of {} i.e. a JSON object)
        }
        ::mongocxx::cursor ResultCursor = coll.find(bsoncxx::from_json("{}"));
        for (const bsoncxx::document::view& Result : ResultCursor) {
            //            qDebug() << bsoncxx::to_json(Result).c_str();
        }
    }
    catch (const exception& e) {
        qDebug() << e.what();
    }

    //    coll.update_one(bsoncxx::from_json(""), bsoncxx::from_json(""));

    //    ::mongocxx::cursor DatabaseInfoCursor = Local.list_databases();
    //    ::mongocxx::options::find find_options{};
    //    find_options.projection(make_document(kvp("_id", 1)));
    //    for (const bsoncxx::v_noabi::document::view& DatabaseInfoDoc: DatabaseInfoCursor) {
    //        qDebug() << "Database:";
    //        qDebug() << bsoncxx::to_json(DatabaseInfoDoc).c_str();
    //        ::mongocxx::database Database = Local[DatabaseInfoDoc["name"].get_utf8().value];
    //        ::mongocxx::cursor CollectionInfoCursor = Database.list_collections();
    //        qDebug() << "Collections:";
    //        for (const bsoncxx::document::view& CollectionInfoDoc: CollectionInfoCursor) {
    //            qDebug() << bsoncxx::to_json(CollectionInfoDoc).c_str();
    //            ::mongocxx::collection Collection = Database[CollectionInfoDoc["name"].get_utf8().value];
    //            ::mongocxx::cursor DocumentCursor = Collection.find(bsoncxx::from_json("{}"), find_options);
    //            qDebug() << "Documents:";
    //            for (const bsoncxx::document::view& Doc: DocumentCursor) {
    //                qDebug() << bsoncxx::to_json(Doc).c_str();
    //            }
    //        }
    //    }

        // drop db
    //    test.drop();

    LastFinishedFn.assign(__FUNCTION__);
}

void WMMTest::DuckX::QuickStart() {
    LastStartedFn.assign(__FUNCTION__);

    duckx::Document doc("test/test-data/刘畅 - 同一首歌.docx");
    doc.open();

    for (auto p = doc.paragraphs(); p.has_next(); p.next())
        for (auto r = p.runs(); r.has_next(); r.next())
            qDebug() << r.get_text().c_str();

    LastFinishedFn.assign(__FUNCTION__);
}
