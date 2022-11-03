#ifndef WRITING_MATERIALS_MANAGER_EXTRAFUNCTIONWINDOW_H
#define WRITING_MATERIALS_MANAGER_EXTRAFUNCTIONWINDOW_H

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

#include "TextArea.h"

namespace WritingMaterialsManager {
    class ExtraFunctionWindow : public QMainWindow {
    public:
        explicit ExtraFunctionWindow(QWidget* const Parent = nullptr);
    private:
        inline static const char* DefaultWindowTitle = "附加功能";

        QWidget* const CentralWidget = new QWidget(this);
        QMenuBar* const MenuBar = new QMenuBar(this);
        QToolBar* const ToolBar = new QToolBar(this);
        QStatusBar* const StatusBar = new QStatusBar(this);

        QTabWidget* const RootView = new QTabWidget;
    };

    class DocumentExtractPage : public QWidget {
    Q_OBJECT
    public:
        inline static const char* DefaultPageTitle = "DOCX/PDF提取";
        inline static const char* DefaultPageTitleSuffix = "DOCX/PDF提取 - ";

        explicit DocumentExtractPage(QWidget* const Parent = nullptr);
        void OpenFile();
        void OpenFile(const QString& FileName);
        QString GetPlainTextFromOpenDocument() const;
    signals:
        void DocumentOpened(const QString& FileName);
    private:
        class DocumentDisplayArea : public TextArea {
        public:
            inline static QAction* Open;

            explicit DocumentDisplayArea();
            void contextMenuEvent(QContextMenuEvent* const E) override;
        };

        DocumentDisplayArea* const DocumentDisplayArea = new class DocumentDisplayArea;
    };
} // WritingMaterialsManager

#endif //WRITING_MATERIALS_MANAGER_EXTRAFUNCTIONWINDOW_H
