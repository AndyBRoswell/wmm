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
#include <QThread>
#include <QWidget>

#include "DatabaseConsole.h"
#include "MongoDBAccessor.h"

namespace WritingMaterialsManager {
    class MongoDBConsole : public DatabaseConsole {
    Q_OBJECT
    public:
        QWidget* const ControlArea;
        QLineEdit* const URLForm;
        QLineEdit* const mongoshCommandForm;
        QPushButton* const ExecuteButton;

        QPlainTextEdit* const CommandForm;

        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

        void ExecuteShellCommand();
    signals:
        void SendShellCommand(const QString& Command);
    private:
        QThread mongoshAccessThread;
    };

    class MongoDBShellAccessor : public QObject {
    Q_OBJECT
    public:
        explicit MongoDBShellAccessor(const QString& mongoshCommand, const QString& MongoDBURL);
        ~MongoDBShellAccessor();
    public slots:
        void Execute(const QString& Command);
    signals:
        void MoreMongoDBShellResult(const QString& Result);
    private:
        std::shared_ptr<QProcess> mongoshProcess;
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
