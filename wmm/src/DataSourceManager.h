#ifndef WRITING_MATERIALS_MANAGER_DATASOURCEMANAGER_H
#define WRITING_MATERIALS_MANAGER_DATASOURCEMANAGER_H

#include <memory>

#include <QMainWindow>
#include <QTreeView>

#include "MongoDBAccessor.h"

namespace WritingMaterialsManager {
    namespace Ui {
        class DataSourceManager;
    }

    class DataSourceManager : public QMainWindow {
    Q_OBJECT

    public:
        explicit DataSourceManager(QWidget* Parent = nullptr);
        ~DataSourceManager();

    private:
        class Page: public QWidget {
        public:
            Page(QWidget* const Parent);
            ~Page();

            QTreeView* TreeView;
        };

        Ui::DataSourceManager* UI;

        MongoDBAccessor MongoDBAccessor;
    };
}

#endif // WRITING_MATERIALS_MANAGER_DATASOURCEMANAGER_H
