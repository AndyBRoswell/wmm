#include "DataSourceManager.h"
#include "ui_DataSourceManager.h"

#include <QGridLayout>

using namespace WritingMaterialsManager;

DataSourceManager::DataSourceManager(QWidget* parent) : QMainWindow(parent), UI(new Ui::DataSourceManager) {
    UI->setupUi(this);

    // preparation
    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(UI->DataSourceTab);
}

DataSourceManager::~DataSourceManager() {
    delete UI;
}
