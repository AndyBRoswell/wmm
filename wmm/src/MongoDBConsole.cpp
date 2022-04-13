#include "MongoDBConsole.h"

#include <QGridLayout>

using namespace WritingMaterialsManager;

/// class MongoDBConsole

MongoDBConsole::MongoDBConsole(QWidget* const parent) : QWidget(parent), RootView(new QSplitter(this)), FunctionArea(new class FunctionArea), ParamEditor(), ParamEditorModel(new QStringListModel()) {
    // preparation
    setLayout(new QGridLayout);


    RootView->addWidget(FunctionArea);
    RootView->addWidget(ParamEditor);

    layout()->addWidget(RootView);
}

MongoDBConsole::~MongoDBConsole() {}

/// class MongoDBConsole::FunctionArea

MongoDBConsole::FunctionArea::FunctionArea(QWidget* const Parent) : QSplitter(Parent) {

}
