#include "MongoDBConsole.h"
#include "ui_MongoDBConsole.h"

MongoDBConsole::MongoDBConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MongoDBConsole)
{
    ui->setupUi(this);
}

MongoDBConsole::~MongoDBConsole()
{
    delete ui;
}
