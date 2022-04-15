#include "MongoDBConsole.h"

#include <chrono>
#include <thread>

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace WritingMaterialsManager {
    MongoDBConsole::MongoDBConsole(QWidget* const Parent) : DatabaseConsole(Parent),
                                                            ControlArea(new QWidget()),
                                                            URLForm(new QLineEdit(MongoDBAccessor::LocalMongoDBURI)),
                                                            mongoshCommandForm(new QLineEdit("mongosh")),
                                                            ExecuteButton(new QPushButton("▶")),
                                                            CommandForm(new QPlainTextEdit("show dbs")) {
        MongoDBShellAccessor* const mongoshAccessor = new class MongoDBShellAccessor(mongoshCommandForm->text(), URLForm->text());
        mongoshAccessor->moveToThread(&mongoshAccessThread);
        qDebug() << connect(&mongoshAccessThread, &QThread::finished, mongoshAccessor, &QObject::deleteLater);
        qDebug() << connect(ExecuteButton, &QPushButton::clicked, this, &MongoDBConsole::ExecuteShellCommand);
        qDebug() << connect(this, &MongoDBConsole::SendShellCommand, mongoshAccessor, &MongoDBShellAccessor::Execute);
        qDebug() << connect(mongoshAccessor, &MongoDBShellAccessor::ShellResultReady, this, &MongoDBConsole::SetTextForAssociatedEditors);
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
    }

    void MongoDBConsole::ExecuteShellCommand() {
        qDebug() << "Attempting to send mongosh command" << CommandForm->toPlainText() << "to MongoDBShellAccessor ...";
        emit SendShellCommand(CommandForm->toPlainText());
        qDebug() << "mongosh command was sent to MongoDBShellAccessor.";
    }
/// ----------------------------------------------------------------

    MongoDBShellAccessor::MongoDBShellAccessor(const QString& mongoshCommand, const QString& MongoDBURL) : mongoshProcess(new QProcess) {
        mongoshProcess->start(mongoshCommand, { MongoDBURL });
        qDebug() << "Waiting for the start of mongoshProcess ...";
        qDebug() << (mongoshProcess->waitForStarted(-1) ? "Started." : "Start failed.");
    }

    MongoDBShellAccessor::~MongoDBShellAccessor() {}

    void MongoDBShellAccessor::Execute(const QString& Command) {
        using namespace std::chrono_literals;

        qDebug() << "MongoDBShellAccessor received mongosh command" << Command;
        mongoshProcess->write(Command.toUtf8());
        qDebug() << "MongoDBShellAccessor sent the received mongosh command.";
        std::this_thread::sleep_for(500ms);
        const QByteArray Result = mongoshProcess->readAllStandardOutput();
        if (Result != "") { emit ShellResultReady(Result); }
        else { qDebug() << "The mongosh returns no result."; }
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
