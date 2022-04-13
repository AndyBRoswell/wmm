#include "MongoDBConsole.h"

#include <QGridLayout>

namespace WritingMaterialsManager {
/// class MongoDBConsole

    MongoDBConsole::MongoDBConsole(QWidget* const parent) : QWidget(parent),
                                                            MongoDBAccessor(new class MongoDBAccessor),
                                                            URLForm(new QPlainTextEdit(MongoDBAccessor::LocalMongoDBURI)),
                                                            DatabaseListView(new QListView),
                                                            CollectionListView(new QListView),
                                                            FunctionComboBox(new QComboBox),
                                                            ExecuteButton(new QPushButton("▶")),
                                                            ParamEditor(new QListView),
                                                            RootView(new QSplitter),
                                                            FunctionArea(new QSplitter) {
        FunctionArea->addWidget(DatabaseListView);
        FunctionArea->addWidget(CollectionListView);
        DatabaseListView->setModel(DatabaseListModel);
        CollectionListView->setModel(CollectionListModel);
        QWidget* const ControlArea = new QWidget;
        FunctionArea->addWidget(ControlArea);
        ControlArea->setLayout(new QGridLayout);
        ControlArea->layout()->addWidget(URLForm);
        QWidget* const ExecutionArea = new QWidget;
        ControlArea->layout()->addWidget(ExecutionArea);
        ExecutionArea->setLayout(new QHBoxLayout);
        ExecutionArea->layout()->addWidget(FunctionComboBox);
        ExecutionArea->layout()->addWidget(ExecuteButton);

        setLayout(new QGridLayout);
        layout()->addWidget(RootView);
    }

    MongoDBConsole::~MongoDBConsole() {}
}
