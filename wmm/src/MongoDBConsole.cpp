#include "MongoDBConsole.h"

#include <QGridLayout>

namespace WritingMaterialsManager {
    MongoDBConsole::MongoDBConsole(QWidget* const parent) : QWidget(parent),
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
        ControlArea->setLayout(new QGridLayout);
        ControlArea->layout()->addWidget(URLForm);
        QWidget* const ExecutionArea = new QWidget;
        QHBoxLayout* const ExecutionAreaLayout = new QHBoxLayout;
        ExecutionArea->setLayout(ExecutionAreaLayout);
        ExecutionArea->layout()->addWidget(FunctionComboBox);
        ExecutionArea->layout()->addWidget(ExecuteButton);
        ControlArea->layout()->addWidget(ExecutionArea);
        ExecutionAreaLayout->setStretch(0, 1);
        ExecutionAreaLayout->setStretch(1, 0);
        FunctionArea->addWidget(ControlArea);

        RootView->setOrientation(Qt::Vertical);
        RootView->addWidget(FunctionArea);
        RootView->addWidget(ParamEditor);

        setLayout(new QGridLayout);
        layout()->addWidget(RootView);
    }

    MongoDBConsole::~MongoDBConsole() {}
}
