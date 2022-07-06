#include "DataSourceManagerWindow.h"

#include <QFileSystemModel>
#include <QGridLayout>
#include <QPushButton>

#include "QtTreeModel.h"

namespace WritingMaterialsManager {
    DataSourceManagerWindow::DataSourceManagerWindow(QWidget* Parent) : QMainWindow(Parent),
                                                                        CentralWidget(new QWidget(this)),
                                                                        MenuBar(new QMenuBar(this)),
                                                                        StatusBar(new QStatusBar(this)),
                                                                        DataSourceTab(new QTabWidget(CentralWidget)) {
        // preparation
        if (objectName().isEmpty() == true) setObjectName(QString::fromUtf8("WritingMaterialsManager__DataSourceManagerWindow"));
        resize(1280, 720);
        setWindowTitle(tr("数据来源管理器"));

        setCentralWidget(CentralWidget);
        centralWidget()->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget()->setLayout(new QGridLayout);
        centralWidget()->layout()->addWidget(DataSourceTab);

        DataSourceTab->setObjectName(QString::fromUtf8("DataSourceTab"));
        DataSourceTab->setGeometry(QRect(0, 0, 1280, 671));
        DataSourceTab->setCurrentIndex(-1);

        MenuBar->setObjectName(QString::fromUtf8("MenuBar"));
        MenuBar->setGeometry(QRect(0, 0, 1280, 21));
        setMenuBar(MenuBar);

        StatusBar->setObjectName(QString::fromUtf8("StatusBar"));
        setStatusBar(StatusBar);

        QMetaObject::connectSlotsByName(this);

        Page* const MongoDBPage = new Page(centralWidget());
        QtTreeModel* MongoDBInfoTree = new QtTreeModel(MongoDBPage);
        MongoDBInfoTree->FromJSON(MongoDBAccessor.GetDBsAndCollsInfo());
        MongoDBPage->TreeView->setModel(MongoDBInfoTree);

        Page* const FileSystemPage = new Page(centralWidget());
        QFileSystemModel* FileSystemTree = new QFileSystemModel(FileSystemPage);
        FileSystemTree->setRootPath(QDir::currentPath());
        FileSystemTree->setFilter(QDir::AllDirs);
        FileSystemPage->TreeView->setModel(FileSystemTree);

        DataSourceTab->addTab(MongoDBPage, "MongoDB");
        DataSourceTab->addTab(FileSystemPage, "File System");
    }

    DataSourceManagerWindow::~DataSourceManagerWindow() {
        while (DataSourceTab->count()) { delete DataSourceTab->widget(0); }
    }

/// ----------------------------------------------------------------

    DataSourceManagerWindow::Page::Page(QWidget* const Parent) : QWidget(Parent), TreeView(new QTreeView(this)) {
        setLayout(new QGridLayout);
        layout()->addWidget(TreeView);
        show();
    }

    DataSourceManagerWindow::Page::~Page() {}
}
