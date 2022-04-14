#include "MongoDBConsole.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace WritingMaterialsManager {
    MongoDBConsole::MongoDBConsole(QWidget* const Parent) : DatabaseConsole(Parent),
                                                            MongoDBAccessor(new class MongoDBAccessor),
                                                            ControlArea(new QWidget()),
                                                            URLForm(new QLineEdit(MongoDBAccessor::LocalMongoDBURI)),
                                                            mongoshCommandForm(new QLineEdit("mongosh")),
                                                            ExecuteButton(new QPushButton("▶")),
                                                            CommandForm(new QPlainTextEdit) {
        ControlArea->setLayout(new QHBoxLayout);
        QWidget* const ControlParamArea = new QWidget;
        ControlParamArea->setLayout(new QVBoxLayout);
        ControlParamArea->layout()->addWidget(URLForm);
        ControlParamArea->layout()->addWidget(mongoshCommandForm);
        ControlArea->layout()->addWidget(ControlParamArea);
        ControlArea->layout()->addWidget(ExecuteButton);

        QVBoxLayout* const MainLayout = new QVBoxLayout;
        MainLayout->setContentsMargins(0, 0, 0, 0);
        setLayout(MainLayout);
        layout()->addWidget(ControlArea);
        layout()->addWidget(CommandForm);
        MainLayout->setStretch(0, 0);
        MainLayout->setStretch(1, 1);
    }

    MongoDBConsole::~MongoDBConsole() {}

    void MongoDBConsole::ExecuteShellCommand() {

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
