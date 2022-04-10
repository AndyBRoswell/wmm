#include "DataSourceManager.h"
#include "ui_DataSourceManager.h"

#include <QFileSystemModel>
#include <QGridLayout>

#include "QtGUITreeModel.h"

using namespace WritingMaterialsManager;

/// DataSourceManager

DataSourceManager::DataSourceManager(QWidget* parent) : QMainWindow(parent), UI(new Ui::DataSourceManager) {
    UI->setupUi(this);

    // preparation
    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(UI->DataSourceTab);

    Page* const MongoDBPage = new Page;
    QtGUITreeModel MongoDBInfoTree(MongoDBPage);
    MongoDBInfoTree.FromJSON(MongoDBAccessor.GetDBsAndCollsInfo());
    MongoDBPage->TreeView->setModel(&MongoDBInfoTree);

    Page* const FileSystemPage = new Page;
    QFileSystemModel FileSystemTree;
    FileSystemTree.setRootPath(QDir::currentPath());
    FileSystemPage->TreeView->setModel(&FileSystemTree);
    FileSystemPage->TreeView->setRootIndex(FileSystemTree.index(QDir::currentPath()));

    UI->DataSourceTab->addTab(MongoDBPage, "MongoDB");
    UI->DataSourceTab->addTab(FileSystemPage, "File System");
}

DataSourceManager::~DataSourceManager() {
    while (UI->DataSourceTab->count()) { delete UI->DataSourceTab->widget(0); }
    delete UI;
}

/// DataSourceManager::Pages

DataSourceManager::Page::Page(): TreeView(new QTreeView(this)) {
    setLayout(new QGridLayout);
    layout()->addWidget(TreeView.get());
}
