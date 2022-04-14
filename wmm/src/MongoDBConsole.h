#ifndef WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
#define WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H

#include <QComboBox>
#include <QListView>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QStringListModel>
#include <QWidget>

#include "DatabaseConsole.h"
#include "MongoDBAccessor.h"

namespace WritingMaterialsManager {
    class MongoDBConsole : public DatabaseConsole {
    public:
        QSplitter* const FunctionArea;

        QPlainTextEdit* const URLForm;
        QListView* const DatabaseListView;
        QListView* const CollectionListView;
        QComboBox* const FunctionComboBox;
        QPushButton* const ExecuteButton;

        QListView* const ParamEditor;

        QSplitter* const RootView;

        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

    private:
        QStringListModel* const DatabaseListModel;
        QStringListModel* const CollectionListModel;
        QStringListModel* const ParamListModel;
        std::shared_ptr<MongoDBAccessor> MongoDBAccessor;
    };
}

#endif // WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
