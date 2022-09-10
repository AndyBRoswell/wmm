#ifndef WRITING_MATERIALS_MANAGER_DATASOURCEMANAGER_H
#define WRITING_MATERIALS_MANAGER_DATASOURCEMANAGER_H

#include <memory>

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QTreeView>

#include "MongoDBAccessor.h"

namespace WritingMaterialsManager {
    class DataSourceManagerWindow : public QMainWindow {
    Q_OBJECT
    public:
        explicit DataSourceManagerWindow(QWidget* Parent = nullptr);
        ~DataSourceManagerWindow();
    private:
        class Page: public QWidget {
        public:
            Page(QWidget* const Parent);
            ~Page();

            QTreeView* TreeView;
        };

        QWidget* const CentralWidget;
        QTabWidget* const DataSourceTab;
        QMenuBar* const MenuBar;
        QStatusBar* const StatusBar;

        MongoDBAccessor MongoDBAccessor;
    };
}

#endif // WRITING_MATERIALS_MANAGER_DATASOURCEMANAGER_H
