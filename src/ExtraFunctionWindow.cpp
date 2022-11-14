#include "ExtraFunctionWindow.h"

#include <mutex>

#include <QApplication>
#include <QGridLayout>
#include <QtGlobal>
#include <QtPdf/QPdfDocument>

#include "duckx.hpp"

namespace WritingMaterialsManager {
    ExtraFunctionWindow::ExtraFunctionWindow(QWidget* const Parent) : QMainWindow(Parent) {
        // initialize basic components
        if (objectName().isEmpty()) setObjectName("WritingMaterialsManager__ExtraFunctionWindow");

        CentralWidget->setObjectName("CentralWidget");
        setCentralWidget(CentralWidget);

        MenuBar->setObjectName("MenuBar");
        setMenuBar(MenuBar);
        
        StatusBar->setObjectName("StatusBar");
        setStatusBar(StatusBar);
        
        ToolBar->setObjectName("ToolBar");
        ToolBar->setWindowTitle(QCoreApplication::translate("WritingMaterialsManager::ExtraFunctionWindow", "ToolBar", nullptr));
        addToolBar(Qt::TopToolBarArea, ToolBar);

        setWindowTitle(tr(DefaultWindowTitle));

        QMetaObject::connectSlotsByName(this);

        // initialize demo pages
        DocumentExtractPage* const DocumentExtractPage = new class DocumentExtractPage(this);
        RootView->addTab(DocumentExtractPage, tr(DocumentExtractPage::DefaultPageTitle));
        connect(DocumentExtractPage, &DocumentExtractPage::DocumentOpened, [=](const QString& FileName) {
            RootView->setTabText(RootView->indexOf(DocumentExtractPage), DocumentExtractPage::DefaultPageTitleSuffix + FileName);
            });

        QGridLayout* const MainLayout = new QGridLayout; {
            MainLayout->setContentsMargins(0, 0, 0, 0);
            MainLayout->addWidget(RootView);
        }
        CentralWidget->setLayout(MainLayout);
    }
/// ----------------------------------------------------------------

    DocumentExtractPage::DocumentExtractPage(QWidget* const Parent) : QWidget(Parent) {
        setLayout(new QGridLayout); {
            layout()->setContentsMargins(0, 0, 0, 0);
            layout()->addWidget(DocumentDisplayArea);
        }

        const auto Connection = connect(DocumentDisplayArea::Open, &QAction::triggered, this, qOverload<>(&DocumentExtractPage::OpenFile));
    }

    QString DocumentExtractPage::GetPlainTextFromOpenDocument() const {
        return DocumentDisplayArea->toPlainText();
    }

    DocumentExtractPage::DocumentDisplayArea::DocumentDisplayArea() {
        static std::once_flag StaticInitialized;
        std::call_once(StaticInitialized, []() {
            Open = new QAction(tr("打开"));
            Open->setShortcut(QKeySequence::Open);
            Open->setStatusTip(tr("打开一个文件"));
        });

        setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); // read-only
    }

    void DocumentExtractPage::DocumentDisplayArea::contextMenuEvent(QContextMenuEvent* const E) {
        QMenu* const ContextMenu = createStandardContextMenu(); {
            ContextMenu->addSeparator();
            ContextMenu->addAction(Open);
            ContextMenu->exec(E->globalPos());
        }
    }

    void DocumentExtractPage::OpenFile() {
        const QString FileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath(), tr("Microsoft DOCX (*.docx);;Portable Document Format(*.pdf)"));
        if (FileName.isEmpty() == false) { OpenFile(FileName); } // selected a file
    }
    
    void DocumentExtractPage::OpenFile(const QString& FileName) {
        const QFileInfo FileInfo(FileName);
        DocumentDisplayArea->clear();
        if (FileInfo.suffix() == "docx") {
            duckx::Document Doc(FileName.toUtf8().constData());
            Doc.open();
            for (auto p = Doc.paragraphs(); p.has_next(); p.next())
                for (auto r = p.runs(); r.has_next(); r.next()) DocumentDisplayArea->appendPlainText(r.get_text().c_str());
        }
        else if (FileInfo.suffix() == "pdf") {
            QPdfDocument Doc;
            Doc.load(FileName);
            for (int i = 0; i < Doc.pageCount(); ++i) DocumentDisplayArea->appendPlainText(Doc.getAllText(i).text());
            Doc.close();
        }

        emit DocumentOpened(FileName);
    }
} // WritingMaterialsManager
