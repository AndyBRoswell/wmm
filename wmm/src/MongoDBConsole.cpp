#include "MongoDBConsole.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace WritingMaterialsManager;

MongoDBConsole::MongoDBConsole(QWidget* const parent) : QWidget(parent){
    // preparation
    setLayout(new QVBoxLayout);
}

MongoDBConsole::~MongoDBConsole() {}
