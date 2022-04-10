#include "DataSourceManager.h"
#include "ui_DataSourceManager.h"

using namespace WritingMaterialsManager;

DataSourceManager::DataSourceManager(QWidget* parent) : QMainWindow(parent), ui(new Ui::DataSourceManager) {
    ui->setupUi(this);
}

DataSourceManager::~DataSourceManager() {
    delete ui;
}
