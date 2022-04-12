#ifndef DATASOURCEMANAGER_H
#define DATASOURCEMANAGER_H

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

            std::shared_ptr<QTreeView> TreeView;
        };

        Ui::DataSourceManager* UI;

        MongoDBAccessor MongoDBAccessor;
    };
}

#endif // DATASOURCEMANAGER_H
