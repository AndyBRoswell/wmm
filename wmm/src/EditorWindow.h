#ifndef WRITING_MATERIALS_MANAGER_EDITORWINDOW_H
#define WRITING_MATERIALS_MANAGER_EDITORWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QSplitter>

namespace WritingMaterialsManager {
    namespace Ui {
        class EditorWindow;
    }

    class EditorWindow : public QMainWindow {
    Q_OBJECT
    public:
        QSplitter* RootView;

        explicit EditorWindow(QWidget* Parent = nullptr);
        ~EditorWindow();
    private:
        class Page : public QWidget {
        public:
            QSplitter* RootView;

            Page(QWidget* const Parent = nullptr);
            ~Page();
        };

        class MongoDBConsoleAndEditorPage : public Page {
        public:
            MongoDBConsoleAndEditorPage(QWidget* const Parent = nullptr);
        };

        class EditorOnlyPage : public Page {
        public:
            EditorOnlyPage(QWidget* const Parent = nullptr);
        };

        inline static const QString DefaultQLabelStyleSheet = "QLabel { color: white; }";

        Ui::EditorWindow* UI;
        QLabel* const FileTypeLabel = new QLabel("<File Type>");
        QLabel* const EncodingLabel = new QLabel("<Encoding>");
    };
}

#endif // WRITING_MATERIALS_MANAGER_EDITORWINDOW_H
