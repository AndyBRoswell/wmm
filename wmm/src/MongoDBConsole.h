#ifndef WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
#define WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H

#include <QComboBox>
#include <QListView>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QSplitter>
#include <QStringListModel>
#include <QWidget>

#include "DatabaseConsole.h"
#include "MongoDBAccessor.h"

namespace WritingMaterialsManager {
    class MongoDBConsole : public DatabaseConsole {
    public:
        QWidget* ControlArea;
        QLineEdit* const URLForm;
        QLineEdit* const mongoshCommandForm;
        QPushButton* const ExecuteButton;

        QPlainTextEdit* const CommandForm;

        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

        void ExecuteShellCommand();
    private:
        std::shared_ptr<QProcess> mongoshProcess;
        std::shared_ptr<MongoDBAccessor> MongoDBAccessor;
    };

    class [[deprecated("Using MongoDBConsole instead.")]] MongoDBLegacyConsole : public DatabaseConsole {
    public:
        QSplitter* const FunctionArea;

        QPlainTextEdit* const URLForm;
        QListView* const DatabaseListView;
        QListView* const CollectionListView;
        QComboBox* const FunctionComboBox;
        QPushButton* const ExecuteButton;

        QListView* const ParamEditor;

        QSplitter* const RootView;

        explicit MongoDBLegacyConsole(QWidget* const Parent = nullptr);
        ~MongoDBLegacyConsole();
    private:
        QStringListModel* const DatabaseListModel;
        QStringListModel* const CollectionListModel;
        QStringListModel* const ParamListModel;
        std::shared_ptr<MongoDBAccessor> MongoDBAccessor;
    };
}

#endif // WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
