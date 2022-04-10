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
        Ui::DataSourceManager* UI;
    };
}

#endif // DATASOURCEMANAGER_H
