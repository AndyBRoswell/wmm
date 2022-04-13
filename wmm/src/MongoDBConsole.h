#ifndef WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
#define WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H

#include <QComboBox>
#include <QListView>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QStringListModel>
#include <QWidget>

#include <MongoDBAccessor.h>

namespace WritingMaterialsManager {
    class MongoDBConsole : public QWidget {
    Q_OBJECT

    public:
        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

        QSplitter* FunctionArea;

        QPlainTextEdit* URLForm;
        QListView* DatabaseListView;
        QListView* CollectionListView;
        QComboBox* FunctionComboBox;
        QPushButton* ExecuteButton;

        QListView* ParamEditor;

        QSplitter* RootView;

    private:
        QStringListModel* DatabaseListModel;
        QStringListModel* CollectionListModel;
        QStringListModel* ParamListModel;
        std::shared_ptr<MongoDBAccessor> MongoDBAccessor;
    };
}

#endif // WRITING_MATERIALS_MANAGER_MONGODBCONSOLE_H
