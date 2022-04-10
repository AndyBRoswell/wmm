#ifndef DATASOURCEMANAGER_H
#define DATASOURCEMANAGER_H

#include <QMainWindow>

namespace WritingMaterialsManager {
    namespace Ui {
        class DataSourceManager;
    }

    class DataSourceManager : public QMainWindow {
    Q_OBJECT

    public:
        explicit DataSourceManager(QWidget* parent = nullptr);
        ~DataSourceManager();

    private:
        Ui::DataSourceManager* ui;
    };
}

#endif // DATASOURCEMANAGER_H
