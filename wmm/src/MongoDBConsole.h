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

#include "MongoDBAccessor.h"
#include "DatabaseConsole.h"
#include "TextArea.h"

namespace WritingMaterialsManager {
    class MongoShAccessor : public QObject {
    Q_OBJECT
    public:
        explicit MongoShAccessor(const QString& mongoshCommand, const QString& MongoDBURL);
        ~MongoShAccessor();

        void SendResult();
    public slots:
        void Execute(const QString& Command);
    signals:
        void MoreResult(const QString& Result);
        void NoMoreResult();
    private:
        std::shared_ptr<QProcess> mongoshProcess;
    };

    class MongoDBConsole : public DatabaseConsole {
    Q_OBJECT
    public:
        QWidget* const ControlArea;
        TextField* const URLForm;
        TextField* const mongoshCommandForm;
        QPushButton* const ExecuteButton;

        TextArea* const CommandForm;

        explicit MongoDBConsole(const QString& mongoshCommand = "mongosh", QWidget* const Parent = nullptr);
        ~MongoDBConsole();

        void ExecuteShellCommand();
        void ArrangeContentViewForAssociatedEditors() override;
    signals:
        void NewShellCommand(const QString& Command);
    private:
        MongoShAccessor mongoshAccessor;
        QThread mongoshAccessThread;
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
