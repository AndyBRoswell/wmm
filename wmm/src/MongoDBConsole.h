#ifndef MONGODBCONSOLE_H
#define MONGODBCONSOLE_H

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
        class FunctionArea : public QSplitter {
        public:
            class ControlArea: public QWidget {
            public:
                QComboBox* FunctionComboBox;
                QPushButton* ExecuteButton;

                ControlArea(QWidget* const Parent = nullptr);
            };

            QPlainTextEdit* URLForm;
            QListView* DatabaseListView;
            QListView* CollectionListView;


            QSplitter* DatabaseListArea;
            ControlArea* ControlArea;

            explicit FunctionArea(QWidget* const Parent = nullptr);
        private:
            QStringListModel* DatabaseListModel;
            QStringListModel* CollectionListModel;
        };

        explicit MongoDBConsole(QWidget* const Parent = nullptr);
        ~MongoDBConsole();

        QSplitter* RootView;
        FunctionArea* FunctionArea;
        QListView* ParamEditor;

    private:
        QStringListModel* ParamEditorModel;
        std::shared_ptr<MongoDBAccessor> MongoDBAccessor;
    };
}

#endif // MONGODBCONSOLE_H
