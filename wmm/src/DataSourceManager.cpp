#include "DataSourceManager.h"
#include "ui_DataSourceManager.h"

#include <QGridLayout>

namespace WMM = WritingMaterialsManager;
using namespace WMM;

DataSourceManager::DataSourceManager(QWidget* parent) : QMainWindow(parent), ui(new WMM::Ui::DataSourceManager) {
    ui->setupUi(this);

    // preparation
    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(ui->DataSourceTab);
}

DataSourceManager::~DataSourceManager() {
    delete ui;
}
