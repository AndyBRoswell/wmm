#include "MongoDBConsole.h"

#include <chrono>

#include <QHBoxLayout>

#include <bsoncxx/json.hpp>
#include <bsoncxx/exception/exception.hpp>

namespace WritingMaterialsManager {
    MongoDBConsole::MongoDBConsole(QWidget* const Parent) : DatabaseConsole(Parent),
                                                            mongoshAccessor(new class MongoShAccessor(mongoshCommandForm->text(), URLForm->text())),
                                                            ControlArea(new QWidget()),
                                                            URLForm(new TextField(MongoDBAccessor::LocalMongoDBURI)),
                                                            mongoshCommandForm(new TextField("mongosh")),
                                                            ExecuteButton(new QPushButton("▶")),
                                                            CommandForm(new TextArea("show dbs\n")) {
        mongoshAccessor->moveToThread(&mongoshAccessThread);
        connect(&mongoshAccessThread, &QThread::finished, mongoshAccessor, &QObject::deleteLater);
        connect(ExecuteButton, &QPushButton::clicked, this, &MongoDBConsole::ExecuteShellCommand);
        connect(this, &MongoDBConsole::SendShellCommand, mongoshAccessor, &MongoShAccessor::Execute);
        connect(mongoshAccessor, &MongoShAccessor::MoreMongoShResult, this, &MongoDBConsole::AppendTextForAssociatedEditors);
        connect(mongoshAccessor, &MongoShAccessor::NoMoreResult, this, &MongoDBConsole::ArrangeContentViewForAssociatedEditors);
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
        layout()->addWidget(ControlArea);
        layout()->addWidget(CommandForm);
        MainLayout->setStretch(0, 0);
        MainLayout->setStretch(1, 1);
    }

    MongoDBConsole::~MongoDBConsole() {
        mongoshAccessThread.quit();
        mongoshAccessThread.wait();
        delete mongoshAccessor;
    }

    void MongoDBConsole::ExecuteShellCommand() {
        SetTextForAssociatedEditors("");
        qDebug() << "Attempting to send mongosh command" << CommandForm->toPlainText() << "to MongoDBShellAccessor ...";
        emit SendShellCommand(CommandForm->toPlainText());
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
            Editor->SetEncoding("UTF-8");
            Editor->SetFileType("MongoDB Extended JSON");
        }
        DatabaseConsole::ArrangeContentViewForAssociatedEditors();
    }

/// ----------------------------------------------------------------

    MongoShAccessor::MongoShAccessor(const QString& mongoshCommand, const QString& MongoDBURL) : mongoshProcess(new QProcess) {
        mongoshProcess->start(mongoshCommand, { MongoDBURL });
        qDebug() << "Waiting for the start of mongoshProcess ...";
        qDebug() << (mongoshProcess->waitForStarted(-1) ? "Started." : "Start failed.");
    }

    MongoShAccessor::~MongoShAccessor() {}

    void MongoShAccessor::Execute(const QString& Command) {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        qDebug() << "MongoDBShellAccessor received mongosh command" << Command;
        mongoshProcess->write(Command.toUtf8());
        qDebug() << "MongoDBShellAccessor sent the received mongosh command.";
        time_point<high_resolution_clock> return_ends_time_point{};
        while (return_ends_time_point.time_since_epoch().count() == 0 || high_resolution_clock::now() - return_ends_time_point <= 1s) {
            const QByteArray Result = mongoshProcess->readAllStandardOutput();
            if (Result != "") { emit MoreMongoShResult(Result); }
            else if (return_ends_time_point.time_since_epoch().count() == 0) return_ends_time_point = high_resolution_clock::now();
        }
        const QByteArray Result = mongoshProcess->readAllStandardOutput();
        emit MoreMongoShResult(Result);
        qDebug() << "No more mongosh result.";
        emit NoMoreResult();
    }

/// ----------------------------------------------------------------

    [[deprecated("Using MongoDBConsole instead.")]]
    MongoDBLegacyConsole::MongoDBLegacyConsole(QWidget* const parent) : DatabaseConsole(parent),
                                                                        MongoDBAccessor(new class MongoDBAccessor),
                                                                        URLForm(new QPlainTextEdit(MongoDBAccessor::LocalMongoDBURI)),
                                                                        DatabaseListView(new QListView),
                                                                        CollectionListView(new QListView),
                                                                        FunctionComboBox(new QComboBox),
                                                                        ExecuteButton(new QPushButton("▶")),
                                                                        ParamEditor(new QListView),
                                                                        RootView(new QSplitter),
                                                                        FunctionArea(new QSplitter),
                                                                        DatabaseListModel(new QStringListModel),
                                                                        CollectionListModel(new QStringListModel),
                                                                        ParamListModel(new QStringListModel) {
        DatabaseListView->setModel(DatabaseListModel);
        CollectionListView->setModel(CollectionListModel);
        ParamEditor->setModel(ParamListModel);

        FunctionArea->addWidget(DatabaseListView);
        FunctionArea->addWidget(CollectionListView);
        QWidget* const ControlArea = new QWidget;
        QGridLayout* const ControlAreaLayout = new QGridLayout;
        ControlArea->setLayout(ControlAreaLayout);
        ControlAreaLayout->addWidget(URLForm);
        QWidget* const ExecutionArea = new QWidget;
        QHBoxLayout* const ExecutionAreaLayout = new QHBoxLayout;
        ExecutionArea->setLayout(ExecutionAreaLayout);
        ExecutionAreaLayout->addWidget(FunctionComboBox);
        ExecutionAreaLayout->addWidget(ExecuteButton);
        ControlAreaLayout->addWidget(ExecutionArea);
        ControlAreaLayout->setColumnStretch(0, 1);
        ControlAreaLayout->setColumnStretch(1, 0);
        ExecutionAreaLayout->setStretch(0, 1);
        ExecutionAreaLayout->setStretch(1, 0);
        FunctionArea->addWidget(ControlArea);
        FunctionArea->setStretchFactor(0, 1);
        FunctionArea->setStretchFactor(1, 1);
        FunctionArea->setStretchFactor(2, 2);

        RootView->setOrientation(Qt::Vertical);
        RootView->addWidget(FunctionArea);
        RootView->addWidget(ParamEditor);
        RootView->setStretchFactor(0, 1);
        RootView->setStretchFactor(1, 2);

        setLayout(new QGridLayout);
        layout()->addWidget(RootView);
    }

    [[deprecated("Using MongoDBConsole instead.")]] MongoDBLegacyConsole::~MongoDBLegacyConsole() {}
}
