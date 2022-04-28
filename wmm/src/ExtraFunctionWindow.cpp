#include "ExtraFunctionWindow.h"

#include <QApplication>

namespace WritingMaterialsManager {
    ExtraFunctionWindow::ExtraFunctionWindow(QWidget* const Parent) : QMainWindow(Parent) {
        // initialize basic components
        if (objectName().isEmpty()) setObjectName("WritingMaterialsManager__ExtraFunctionWindow");
        resize(1280, 720);

        CentralWidget->setObjectName("CentralWidget");
        setCentralWidget(CentralWidget);
        MenuBar->setObjectName("MenuBar");
        MenuBar->setGeometry(0, 0, 1280, 21);
        setMenuBar(MenuBar);
        StatusBar->setObjectName("StatusBar");
        setStatusBar(StatusBar);
        ToolBar->setObjectName("ToolBar");
        addToolBar(Qt::TopToolBarArea, ToolBar);

        setWindowTitle(tr(DefaultWindowTitle));
        ToolBar->setWindowTitle(QCoreApplication::translate("WritingMaterialsManager::ExtraFunctionWindow", "ToolBar", nullptr));

        QMetaObject::connectSlotsByName(this);
    }
} // WritingMaterialsManager
