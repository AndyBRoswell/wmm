#include "ExtraFunctionWindow.h"

#include <mutex>

#include <QApplication>
#include <QGridLayout>
#include <QtPdf/QPdfDocument>

#include "duckx.hpp"

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

        // initialize demo pages
        RootView->addTab(new DocumentExtractPage(this), "DOCX/PDF Extract");

        QGridLayout* const MainLayout = new QGridLayout;
        CentralWidget->setLayout(MainLayout);
        MainLayout->setContentsMargins(0, 0, 0, 0);
        MainLayout->addWidget(RootView);
    }

/// ----------------------------------------------------------------

    ExtraFunctionWindow::DocumentExtractPage::DocumentExtractPage(ExtraFunctionWindow* const OuterInstance, QWidget* const Parent) : QWidget(Parent),
                                                                                                                                     thisAtExtraFunctionWindow(OuterInstance) {

        setLayout(new QGridLayout);
        layout()->setContentsMargins(0, 0, 0, 0);
        layout()->addWidget(DocumentDisplayArea);

        const auto Connection = connect(DocumentDisplayArea::Open, &QAction::triggered, this, &DocumentExtractPage::OpenFile);
    }

    ExtraFunctionWindow::DocumentExtractPage::DocumentDisplayArea::DocumentDisplayArea(DocumentExtractPage* const OuterInstance) : thisAtDocumentExtractPage(OuterInstance) {
        static std::once_flag StaticInitialized;
        std::call_once(StaticInitialized, []() {
            Open = new QAction(tr("打开"));
            Open->setShortcut(QKeySequence::Open);
            Open->setStatusTip(tr("打开一个文件"));
        });

        setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); // read-only
    }

    void ExtraFunctionWindow::DocumentExtractPage::DocumentDisplayArea::contextMenuEvent(QContextMenuEvent* const E) {
        QMenu* const ContextMenu = createStandardContextMenu();
        ContextMenu->addSeparator();
        ContextMenu->addAction(Open);
        ContextMenu->exec(E->globalPos());
    }

    void ExtraFunctionWindow::DocumentExtractPage::OpenFile() {
        const QString FileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath(), tr("Microsoft DOCX (*.docx);;Portable Document Format(*.pdf)"));
        if (FileName.isEmpty() == false) {
            QFileInfo FileInfo(FileName);
            if (FileInfo.suffix() == "docx") {
                DocumentDisplayArea->clear();
                duckx::Document Doc(FileName.toUtf8().constData());
                Doc.open();
                for (auto p = Doc.paragraphs(); p.has_next(); p.next())
                    for (auto r = p.runs(); r.has_next(); r.next()) DocumentDisplayArea->appendPlainText(r.get_text().c_str());
            }
            else if (FileInfo.suffix() == "pdf") {
                DocumentDisplayArea->clear();
                QPdfDocument Doc;
                Doc.load(FileName);
                for (int i = 0; i < Doc.pageCount(); ++i) DocumentDisplayArea->appendPlainText(Doc.getAllText(i).text());
            }
        }
    }
} // WritingMaterialsManager
