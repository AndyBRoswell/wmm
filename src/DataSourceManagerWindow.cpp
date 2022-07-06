#include "DataSourceManagerWindow.h"
#include "ui_DataSourceManagerWindow.h"

#include <QFileSystemModel>
#include <QGridLayout>
#include <QPushButton>

#include "QtTreeModel.h"

namespace WritingMaterialsManager {
    DataSourceManagerWindow::DataSourceManagerWindow(QWidget* Parent) : QMainWindow(Parent), UI(new Ui::DataSourceManagerWindow) {
        UI->setupUi(this);

        // preparation
        setWindowTitle(tr("数据来源管理器"));

        centralWidget()->setLayout(new QGridLayout);
        centralWidget()->layout()->addWidget(UI->DataSourceTab);

        Page* const MongoDBPage = new Page(centralWidget());
        QtTreeModel* MongoDBInfoTree = new QtTreeModel(MongoDBPage);
        MongoDBInfoTree->FromJSON(MongoDBAccessor.GetDBsAndCollsInfo());
        MongoDBPage->TreeView->setModel(MongoDBInfoTree);

        Page* const FileSystemPage = new Page(centralWidget());
        QFileSystemModel* FileSystemTree = new QFileSystemModel(FileSystemPage);
        FileSystemTree->setRootPath(QDir::currentPath());
        FileSystemTree->setFilter(QDir::AllDirs);
        FileSystemPage->TreeView->setModel(FileSystemTree);
//    FileSystemPage->TreeView->setRootIndex(FileSystemTree->index(QDir::currentPath()));

        UI->DataSourceTab->addTab(MongoDBPage, "MongoDB");
        UI->DataSourceTab->addTab(FileSystemPage, "File System");
    }

    DataSourceManagerWindow::~DataSourceManagerWindow() {
        while (UI->DataSourceTab->count()) { delete UI->DataSourceTab->widget(0); }
        delete UI;
    }

/// ----------------------------------------------------------------

    DataSourceManagerWindow::Page::Page(QWidget* const Parent) : QWidget(Parent), TreeView(new QTreeView(this)) {
        setLayout(new QGridLayout);
        layout()->addWidget(TreeView);
        show();
    }

    DataSourceManagerWindow::Page::~Page() {}
}
