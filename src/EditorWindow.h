#ifndef WRITING_MATERIALS_MANAGER_EDITORWINDOW_H
#define WRITING_MATERIALS_MANAGER_EDITORWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QSplitter>

namespace WritingMaterialsManager {
    class EditorWindow : public QMainWindow {
    Q_OBJECT
    public:
        QSplitter* RootView;

        explicit EditorWindow(QWidget* const Parent = nullptr);
        ~EditorWindow();
    public slots:
        void UpdateWindowTitleWithPathName();
        void UpdateWindowTitleWithSuffix(const QString& Suffix = {});
        void UpdateFileTypeLabel();
        void UpdateFileTypeLabel(const QString& FileType);
        void UpdateCharsetLabel();
        void UpdateCharsetLabel(const QString& Charset);
    private:
        class Page : public QWidget {
        public:
            QSplitter* RootView;

            explicit Page(EditorWindow* const OuterInstance, QWidget* const Parent = nullptr);
            ~Page();
        protected:
            EditorWindow* const thisAtEditorWindow;
        };

        class MongoConAndEditorPage : public Page {
        public:
            explicit MongoConAndEditorPage(EditorWindow* const OuterInstance, QWidget* const Parent = nullptr);
        };

        class AnotherMongoConAndEditorPage : public Page {
        public:
            explicit AnotherMongoConAndEditorPage(EditorWindow* const OuterInstance, QWidget* const Parent = nullptr);
        };

        class EditorOnlyPage : public Page {
        public:
            explicit EditorOnlyPage(EditorWindow* const OuterInstance, QWidget* const Parent = nullptr);
        };

        class PythonInteractorPage : public Page {
        public:
            explicit PythonInteractorPage(EditorWindow* const OuterInstance, QWidget* const Parent = nullptr);
        };

        inline static const char* DefaultWindowTitle = "编辑器";
        inline static const char* DefaultWindowTitlePrefix = "编辑器 - ";
        inline static const QString DefaultQLabelStyleSheet = "QLabel { color: white; }";

        QWidget* const centralwidget;
        QMenuBar* const MenuBar;
        QStatusBar* const StatusBar;
        QToolBar* const ToolBar;
        QLabel* const FileTypeLabel = new QLabel("<File Type>");
        QLabel* const CharsetLabel = new QLabel("<Charset>");
    };
}

#endif // WRITING_MATERIALS_MANAGER_EDITORWINDOW_H
