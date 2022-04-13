#include "MongoDBConsole.h"

#include <QGridLayout>

using namespace WritingMaterialsManager;

/// class MongoDBConsole

MongoDBConsole::MongoDBConsole(QWidget* const parent) : QWidget(parent),
                                                        RootView(new QSplitter(this)),
                                                        FunctionArea(new class FunctionArea),
                                                        ParamEditor(new QListView()),
                                                        ParamEditorModel(new QStringListModel) {
    setLayout(new QGridLayout);

    ParamEditor->setModel(ParamEditorModel);

    RootView->setOrientation(Qt::Vertical);
    RootView->addWidget(FunctionArea);
    RootView->addWidget(ParamEditor);

    layout()->addWidget(RootView);
}

MongoDBConsole::~MongoDBConsole() {}

/// class MongoDBConsole::FunctionArea

MongoDBConsole::FunctionArea::FunctionArea(QWidget* const Parent) : QSplitter(Parent),
                                                                    URLForm(new QPlainTextEdit(MongoDBAccessor::LocalMongoDBURI)),
                                                                    DatabaseListView(new QListView),
                                                                    CollectionListView(new QListView),

                                                                    DatabaseListArea(new QSplitter(this)),
                                                                    ControlArea(new class ControlArea),
                                                                    DatabaseListModel(new QStringListModel),
                                                                    CollectionListModel(new QStringListModel) {
    DatabaseListArea->addWidget(DatabaseListView);
    DatabaseListArea->addWidget(CollectionListView);

}

/// class MongoDBConsole::FunctionArea::ControlArea

MongoDBConsole::FunctionArea::ControlArea::ControlArea(QWidget* const Parent) : QWidget(Parent),
                                                                                FunctionComboBox(new QComboBox),
                                                                                ExecuteButton(new QPushButton("▶")) {
    
}
