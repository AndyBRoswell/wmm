#ifndef WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
#define WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H

#include <functional>

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
        std::shared_ptr<QProcess> mongoshProcess = std::make_shared<QProcess>();
    };

    class MongoDBConsole : public DatabaseConsole {
    Q_OBJECT
    public:
        QWidget* const ControlArea = new QWidget;
        TextField* const URLForm = new TextField(MongoDBAccessor::LocalMongoDBURI);
        TextField* const mongoshCommandForm;
        QPushButton* const ExecuteButton = new QPushButton("▶");
        TextArea* const CommandForm = new TextArea("show dbs\n");

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

    class AnotherMongoDBConsole : public DatabaseConsole {
    public:
        TextField* const URLForm = new TextField(MongoDBAccessor::LocalMongoDBURI);
        TextField* const DatabaseNameForm = new TextField("test");
        TextField* const CollectionNameForm = new TextField("coll");
        QComboBox* const FunctionComboBox = new QComboBox;
        QPushButton* const ExecuteButton = new QPushButton("▶");
        QListView* const ParamListView = new QListView;

        explicit AnotherMongoDBConsole(QWidget* const Parent = nullptr);
        ~AnotherMongoDBConsole();
    private:
        QStringListModel ParamListModel;
        std::shared_ptr<MongoDBAccessor> MongoDBAccessor;

        void ExecuteFunction();
    };
}

#endif // WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
