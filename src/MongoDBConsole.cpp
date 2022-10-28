#include "MongoDBConsole.h"

#include <chrono>

#include <QHBoxLayout>

#include <bsoncxx/json.hpp>
#include <bsoncxx/exception/exception.hpp>

namespace WritingMaterialsManager {
    MongoDBConsole::MongoDBConsole(const QString& mongoshCommand, QWidget* const Parent) : 
        DatabaseConsole(Parent), mongoshAccessor(mongoshCommandForm->text(), URLForm->text()), mongoshCommandForm(new TextField(mongoshCommand)) {
        // event handlers regarding mongosh accessor
        mongoshAccessor.moveToThread(&mongoshAccessThread); // or mongoshAccessor will run at the UI thread
        connect(&mongoshAccessThread, &QThread::finished, &mongoshAccessor, &QObject::deleteLater);
        connect(ExecuteButton, &QPushButton::clicked, this, &MongoDBConsole::ExecuteShellCommand);
        connect(this, &MongoDBConsole::NewShellCommand, &mongoshAccessor, &MongoShAccessor::Execute);
        connect(&mongoshAccessor, &MongoShAccessor::MoreResult, this, &MongoDBConsole::AppendTextForAssociatedEditors);
        connect(&mongoshAccessor, &MongoShAccessor::NoMoreResult, this, &MongoDBConsole::ArrangeContentViewForAssociatedEditors);
        mongoshAccessThread.start();

        ControlArea->setLayout(new QHBoxLayout);
        ControlArea->layout()->setContentsMargins(0, 0, 0, 0);
        QWidget* const CtrlParamArea = new QWidget;
        QVBoxLayout* const CtrlParamAreaLayout = new QVBoxLayout;
        CtrlParamAreaLayout->setContentsMargins(0, 0, 0, 0);
        CtrlParamAreaLayout->setSpacing(2);
        CtrlParamAreaLayout->addWidget(URLForm);
        CtrlParamAreaLayout->addWidget(mongoshCommandForm);
        CtrlParamArea->setLayout(CtrlParamAreaLayout);
        ControlArea->layout()->addWidget(CtrlParamArea);
        ControlArea->layout()->addWidget(ExecuteButton);

        QVBoxLayout* const MainLayout = new QVBoxLayout;
        MainLayout->setContentsMargins(0, 0, 0, 0);
        MainLayout->setSpacing(2);
        setLayout(MainLayout);
        MainLayout->addWidget(ControlArea);
        MainLayout->addWidget(CommandForm);
        MainLayout->setStretch(0, 0);
        MainLayout->setStretch(1, 1);

        emit NewShellCommand("");
    }

    MongoDBConsole::~MongoDBConsole() {
        mongoshAccessThread.quit();
        mongoshAccessThread.wait();
    }

    void MongoDBConsole::ExecuteShellCommand() {
        SetTextForAssociatedEditors("");
        RefreshAssociatedEditors();
        // Here we don't call MongoShAccessor::Execute(const QString& Command) directly.
        // Instead, we sent another signal 'cause parameters of connected signals and slots must match 
        // and QPushButton::clicked() can't carry a string (which is for the command to be executed).
        qDebug() << "Attempting to send mongosh command" << CommandForm->toPlainText() << "to MongoDBShellAccessor ...";
        emit NewShellCommand(CommandForm->toPlainText());
        qDebug() << "mongosh command was sent to MongoDBShellAccessor.";
    }

    void MongoDBConsole::ArrangeContentViewForAssociatedEditors() {
        for (auto* Editor: AssociatedEditors) {
            Editor->RawView->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor); // move to the end
            Editor->RawView->textCursor().deletePreviousChar(); // delete the blank last line
            Editor->RawView->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor); // move to the start of the last line
            Editor->RawView->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor); // drag to the end
            Editor->RawView->textCursor().removeSelectedText();
            Editor->RawView->update(); // immediately apply the modification before text (e.g., JSON) parser reads the text from the QPlainTextEdit RawView.
            try {
                const auto Doc = bsoncxx::from_json(Editor->RawView->toPlainText().toUtf8().constData());
                Editor->RawView->setPlainText(QString::fromUtf8(bsoncxx::to_json(Doc, bsoncxx::ExtendedJsonMode::k_relaxed)));
                Editor->RawView->update();
            }
            catch (const bsoncxx::exception& e) {
                qDebug() << "Exception at " << __FUNCTION__ << ": Parsing ERROR when converting to JSON in strict syntax.";
                qDebug() << e.what();
            }
            catch (const std::exception& e) {
                qDebug() << "Exception at " << __FUNCTION__ << ": Parsing ERROR when converting to JSON in strict syntax.";
                qDebug() << e.what();
            }
            Editor->SetCharset("UTF-8");
            Editor->SetFileType("MongoDB Extended JSON");
        }
        DatabaseConsole::ArrangeContentViewForAssociatedEditors();
    }
/// ----------------------------------------------------------------

    MongoShAccessor::MongoShAccessor(const QString& mongoshCommand, const QString& MongoDBURL) {
        mongoshProcess->start(mongoshCommand, { MongoDBURL });
        qDebug() << "Waiting for the start of mongoshProcess ...";
        qDebug() << (mongoshProcess->waitForStarted(-1) ? "Started." : "Start failed.");
    }

    MongoShAccessor::~MongoShAccessor() {}

    void MongoShAccessor::SendResult() {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        time_point<high_resolution_clock> return_ends_time_point{};
        while (return_ends_time_point.time_since_epoch().count() == 0 || high_resolution_clock::now() - return_ends_time_point <= 1s) {
            const QByteArray Result = mongoshProcess->readAllStandardOutput();
            if (Result != "") { emit MoreResult(Result); }
            else if (return_ends_time_point.time_since_epoch().count() == 0) return_ends_time_point = high_resolution_clock::now();
        }
        const QByteArray Result = mongoshProcess->readAllStandardOutput();
        emit MoreResult(Result);
        qDebug() << "No more mongosh result.";
        emit NoMoreResult();
    }

    void MongoShAccessor::Execute(const QString& Command) {
        qDebug() << "MongoDBShellAccessor received mongosh command" << Command;
        mongoshProcess->write(Command.toUtf8());
        qDebug() << "MongoDBShellAccessor sent the received mongosh command.";
        SendResult();
    }
/// ----------------------------------------------------------------

    AnotherMongoDBConsole::AnotherMongoDBConsole(QWidget* const Parent) : DatabaseConsole(Parent) {
        ParamListView->setModel(&ParamListModel);
        connect(ExecuteButton, &QPushButton::clicked, this, &AnotherMongoDBConsole::ExecuteFunction);

        QWidget* const CtrlArea = new QWidget;
        QVBoxLayout* const CtrlAreaLayout = new QVBoxLayout;
        CtrlAreaLayout->setContentsMargins(0, 0, 0, 0);
        CtrlAreaLayout->setSpacing(2);
        CtrlArea->setLayout(CtrlAreaLayout);

        QWidget* const ExecArea = new QWidget;
        QHBoxLayout* const ExecAreaLayout = new QHBoxLayout;
        ExecAreaLayout->setContentsMargins(0, 0, 0, 0);
        ExecArea->setLayout(ExecAreaLayout);
        QSplitter* const ExecParamArea = new QSplitter;
        ExecParamArea->addWidget(DatabaseNameForm);
        ExecParamArea->addWidget(CollectionNameForm);
        ExecParamArea->addWidget(FunctionComboBox);
        ExecAreaLayout->addWidget(ExecParamArea);
        ExecAreaLayout->addWidget(ExecuteButton);
        ExecAreaLayout->setStretch(1, 0);

        CtrlAreaLayout->addWidget(URLForm);
        CtrlAreaLayout->addWidget(ExecArea);

        QVBoxLayout* const MainLayout = new QVBoxLayout;
        MainLayout->setContentsMargins(0, 0, 0, 0);
        MainLayout->setSpacing(2);
        setLayout(MainLayout);
        MainLayout->addWidget(CtrlArea);
        MainLayout->addWidget(ParamListView);
        MainLayout->setStretch(0, 0);
    }

    AnotherMongoDBConsole::~AnotherMongoDBConsole() {}

    void AnotherMongoDBConsole::ExecuteFunction() {
        static mongocxx::database Database;
        static mongocxx::collection Collection;
        static QByteArray LastAccessedDatabaseName;
        static QByteArray LastAccessedCollectionName;
        const QByteArray CurrentDatabaseName = DatabaseNameForm->text().toUtf8();
        const QByteArray CurrentCollectionName = CollectionNameForm->text().toUtf8();
        if (CurrentDatabaseName != LastAccessedDatabaseName) {}
    }
}
