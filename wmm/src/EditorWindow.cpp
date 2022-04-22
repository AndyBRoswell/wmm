#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QApplication>
#include <QGridLayout>

#include "MongoDBConsole.h"
#include "PythonInteractor.h"
#include "TreeEditor.h"

namespace WritingMaterialsManager {
    EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent),
                                                  UI(new Ui::EditorWindow),
                                                  RootView(new QSplitter(this)) {
        UI->setupUi(this);

        QTabWidget* const TabView = new QTabWidget;
        auto* const MDBCPage = new MongoConAndEditorPage(this);
        auto* const EditorPage = new EditorOnlyPage(this);
        auto* const PyInteractorPage = new PythonInteractorPage(this);
        TabView->addTab(MDBCPage, "MongoDB Console");
        TabView->addTab(EditorPage, "TreeEditor Only");
        TabView->addTab(PyInteractorPage, "Python Interactor");
        RootView->addWidget(TabView);

        FileTypeLabel->setStyleSheet(DefaultQLabelStyleSheet);
        CharsetLabel->setStyleSheet(DefaultQLabelStyleSheet);
        UI->StatusBar->addPermanentWidget(CharsetLabel);
        UI->StatusBar->addPermanentWidget(FileTypeLabel);

        setWindowTitle(tr(DefaultWindowTitle));
        centralWidget()->setLayout(new QGridLayout);
        centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
        centralWidget()->layout()->addWidget(RootView);
        show();
    }

    EditorWindow::~EditorWindow() {
        delete UI;
    }

    void EditorWindow::UpdateWindowTitleWithPathName() {
        if (focusWidget() == sender()) setWindowTitle(tr(DefaultWindowTitlePrefix) + static_cast<TreeEditor*>(sender())->GetPathName());
    }
    void EditorWindow::UpdateWindowTitleWithSuffix(const QString& Suffix) {
        if (focusWidget() == sender()) setWindowTitle(tr(DefaultWindowTitlePrefix) + Suffix);
    }

    void EditorWindow::UpdateFileTypeLabel() {
        if (focusWidget() == sender()) FileTypeLabel->setText(static_cast<TreeEditor*>(sender())->GetFileType());
    }
    void EditorWindow::UpdateFileTypeLabel(const QString& FileType) { FileTypeLabel->setText(FileType); }

    void EditorWindow::UpdateCharsetLabel() {
        if (focusWidget() == sender()) CharsetLabel->setText(static_cast<TreeEditor*>(sender())->GetCharset());
    }
    void EditorWindow::UpdateCharsetLabel(const QString& Charset) { CharsetLabel->setText(Charset); }

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

    EditorWindow::MongoConAndEditorPage::MongoConAndEditorPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        MongoDBConsole* const Console = new MongoDBConsole;
        TreeEditor* const Editor = new TreeEditor;
        Console->AddAssociatedEditor(Editor);
        auto ShowPlainTextFn = [=, this]() {
            this->thisAtEditorWindow->UpdateFileTypeLabel("Plain Text");
            this->thisAtEditorWindow->UpdateCharsetLabel("Unicode");
        };
        connect(Console->URLForm, &TextField::MouseDown, thisAtEditorWindow, ShowPlainTextFn);
        connect(Console->mongoshCommandForm, &TextField::MouseDown, thisAtEditorWindow, ShowPlainTextFn);
        connect(Console->CommandForm, &TextArea::MouseDown, thisAtEditorWindow,
                [=, this]() {
                    this->thisAtEditorWindow->UpdateFileTypeLabel("JavaScript");
                    this->thisAtEditorWindow->UpdateCharsetLabel("UTF-8");
                });
        connect(Editor, &TreeEditor::ShouldUpdateFileType, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateFileTypeLabel));
        connect(Editor, &TreeEditor::ShouldUpdateCharset, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateCharsetLabel));
        RootView->addWidget(Console);
        RootView->addWidget(Editor);
        RootView->setStretchFactor(0, 1);
        RootView->setStretchFactor(1, 4);
    }

    void EditorWindow::MongoConAndEditorPage::mousePressEvent(QMouseEvent* E) {
        thisAtEditorWindow->UpdateWindowTitleWithSuffix("MongoDB Console");
    }

/// ----------------------------------------------------------------

    EditorWindow::EditorOnlyPage::EditorOnlyPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        TreeEditor* const Editor = new TreeEditor;
        connect(Editor, &TreeEditor::ShouldUpdatePathName, thisAtEditorWindow, &EditorWindow::UpdateWindowTitleWithPathName);
        connect(Editor, &TreeEditor::ShouldUpdateFileType, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateFileTypeLabel));
        connect(Editor, &TreeEditor::ShouldUpdateCharset, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateCharsetLabel));
        RootView->addWidget(Editor);
    }

/// ----------------------------------------------------------------

    EditorWindow::PythonInteractorPage::PythonInteractorPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        auto* const PyInteractor = new PythonInteractor;
        connect(PyInteractor->PyCommandForm, &TextField::MouseDown, this,
                [OuterInstance]() {
                    OuterInstance->UpdateFileTypeLabel("Plain Text");
                    OuterInstance->UpdateCharsetLabel("Unicode");
                });
        connect(PyInteractor->CodeArea, &TextArea::MouseDown, this,
                [OuterInstance]() {
                    OuterInstance->UpdateFileTypeLabel("Python");
                    OuterInstance->UpdateCharsetLabel("Unicode");
                });
        connect(PyInteractor->ResultArea, &TextArea::MouseDown, this,
                [OuterInstance]() {
                    OuterInstance->UpdateFileTypeLabel("Plain Text");
                    OuterInstance->UpdateCharsetLabel("<OS default charset>");
                });
        RootView->addWidget(PyInteractor);
    }

    void EditorWindow::PythonInteractorPage::mousePressEvent(QMouseEvent* E) {
        thisAtEditorWindow->UpdateWindowTitleWithSuffix("Python Interactor");
    }
}
