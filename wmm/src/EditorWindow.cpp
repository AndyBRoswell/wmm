#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QApplication>
#include <QGridLayout>

#include "Editor.h"
#include "MongoDBConsole.h"

namespace WritingMaterialsManager {
    EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent),
                                                  UI(new Ui::EditorWindow),
                                                  RootView(new QSplitter(this)) {
        UI->setupUi(this);

        QTabWidget* const TabView = new QTabWidget;
        auto* const MDBCPage = new MongoDBConsoleAndEditorPage(this);
        auto* const EditorPage = new EditorOnlyPage(this);
        TabView->addTab(MDBCPage, "MongoDB Console");
        TabView->addTab(EditorPage, "Editor Only");
        RootView->addWidget(TabView);

        FileTypeLabel->setStyleSheet(DefaultQLabelStyleSheet);
        EncodingLabel->setStyleSheet(DefaultQLabelStyleSheet);
        UI->StatusBar->addPermanentWidget(EncodingLabel);
        UI->StatusBar->addPermanentWidget(FileTypeLabel);

        setWindowTitle(tr("编辑器"));
        centralWidget()->setLayout(new QGridLayout);
        centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
        centralWidget()->layout()->addWidget(RootView);
        show();
    }

    EditorWindow::~EditorWindow() {
        delete UI;
    }

    void EditorWindow::UpdateFileTypeLabel() { FileTypeLabel->setText(static_cast<Editor*>(sender())->GetFileType()); }
    void EditorWindow::UpdateFileTypeLabel(const QString& FileType) { FileTypeLabel->setText(FileType); }
    void EditorWindow::UpdateEncodingLabel() { EncodingLabel->setText(static_cast<Editor*>(sender())->GetEncoding()); }
    void EditorWindow::UpdateEncodingLabel(const QString& Encoding) { EncodingLabel->setText(Encoding); }

/// ----------------------------------------------------------------

    EditorWindow::Page::Page(EditorWindow* const OuterInstance, QWidget* const Parent) : thisAtEditorWindow(OuterInstance),
                                                                                         RootView(new QSplitter(this)) {
        RootView->setOrientation(Qt::Vertical);

        setLayout(new QGridLayout);
        layout()->setContentsMargins(0, 0, 0, 0);
        layout()->addWidget(RootView);
    }

    EditorWindow::Page::~Page() {}

/// ----------------------------------------------------------------

    EditorWindow::MongoDBConsoleAndEditorPage::MongoDBConsoleAndEditorPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        MongoDBConsole* const Console = new MongoDBConsole;
        Editor* const Editor = new class Editor;
        Console->AddAssociatedEditor(Editor);
        connect(Console->CommandForm, &TextArea::MouseDown, thisAtEditorWindow,
                [=, this]() {
                    this->thisAtEditorWindow->UpdateFileTypeLabel("JavaScript");
                    this->thisAtEditorWindow->UpdateEncodingLabel("UTF-8");
                }
        );
        connect(Editor, &Editor::ShouldUpdateFileType, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateFileTypeLabel));
        connect(Editor, &Editor::ShouldUpdateEncoding, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateEncodingLabel));
        RootView->addWidget(Console);
        RootView->addWidget(Editor);
        RootView->setStretchFactor(0, 1);
        RootView->setStretchFactor(1, 4);
    }

    EditorWindow::EditorOnlyPage::EditorOnlyPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        Editor* const Editor = new class Editor;
        RootView->addWidget(Editor);
        connect(Editor, &Editor::ShouldUpdateFileType, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateFileTypeLabel));
        connect(Editor, &Editor::ShouldUpdateEncoding, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateEncodingLabel));
    }
}
