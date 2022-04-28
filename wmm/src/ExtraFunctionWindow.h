#ifndef WRITING_MATERIALS_MANAGER_EXTRAFUNCTIONWINDOW_H
#define WRITING_MATERIALS_MANAGER_EXTRAFUNCTIONWINDOW_H

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
        class DOCXExtractPage : public QWidget {
        public:
            explicit DOCXExtractPage(ExtraFunctionWindow* const OuterInstance, QWidget* const Parent = nullptr);
        protected:
            ExtraFunctionWindow* const thisAtExtraFunctionWindow;
        private:
        };

        class PDFExtractPage : public QWidget {

        };

        inline static const char* DefaultWindowTitle = "附加功能";

        QWidget* const CentralWidget = new QWidget(this);
        QMenuBar* const MenuBar = new QMenuBar(this);
        QToolBar* const ToolBar = new QToolBar(this);
        QStatusBar* const StatusBar = new QStatusBar(this);

        QTabWidget* const RootView = new QTabWidget;
    };
} // WritingMaterialsManager

#endif //WRITING_MATERIALS_MANAGER_EXTRAFUNCTIONWINDOW_H
