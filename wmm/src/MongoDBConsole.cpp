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

        setLayout(new QGridLayout);
        layout()->addWidget(RootView);
    }

    MongoDBConsole::~MongoDBConsole() {}
}
