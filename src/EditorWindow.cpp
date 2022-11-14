#include "EditorWindow.h"

#include <QApplication>
#include <QGridLayout>

#include "MongoDBConsole.h"
#include "PythonInteractor.h"
#include "TreeEditor.h"

namespace WritingMaterialsManager {
    EditorWindow::EditorWindow(QWidget* const Parent) : 
        QMainWindow(Parent), centralwidget(new QWidget(this)), MenuBar(new QMenuBar(this)), StatusBar(new QStatusBar(this)), ToolBar(new QToolBar(this)), RootView(new QSplitter(this)) {
        if (objectName().isEmpty() == true) setObjectName(QString::fromUtf8("WritingMaterialsManager__EditorWindow"));

        MenuBar->setObjectName(QString::fromUtf8("MenuBar"));
        MenuBar->setGeometry(QRect(0, 0, 1280, 21));
        setMenuBar(MenuBar);

        FileTypeLabel->setStyleSheet(DefaultQLabelStyleSheet);
        CharsetLabel->setStyleSheet(DefaultQLabelStyleSheet);
        StatusBar->addPermanentWidget(CharsetLabel);
        StatusBar->addPermanentWidget(FileTypeLabel);
        StatusBar->setObjectName(QString::fromUtf8("StatusBar"));
        setStatusBar(StatusBar);

        ToolBar->setObjectName(QString::fromUtf8("ToolBar"));
        ToolBar->setWindowTitle(QCoreApplication::translate("WritingMaterialsManager::EditorWindow", "ToolBar", nullptr));
        addToolBar(Qt::TopToolBarArea, ToolBar);

        QTabWidget* const TabView = new QTabWidget; {
            auto* const MDBCPage = new MongoConAndEditorPage(this);
            auto* const EditorPage = new EditorOnlyPage(this);
            auto* const PyInteractorPage = new PythonInteractorPage(this);
            TabView->addTab(MDBCPage, "MongoDB Console");
            TabView->addTab(EditorPage, "Tree Editor Only");
            TabView->addTab(PyInteractorPage, "Python Interactor");
        }
        RootView->addWidget(TabView);

        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setLayout(new QGridLayout);
        centralwidget->layout()->setContentsMargins(0, 0, 0, 0);
        centralwidget->layout()->addWidget(RootView);
        setCentralWidget(centralwidget);

        setWindowTitle(tr(DefaultWindowTitle));
    }

    EditorWindow::~EditorWindow() {}

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

    EditorWindow::Page::Page(EditorWindow* const OuterInstance, QWidget* const Parent) : 
        thisAtEditorWindow(OuterInstance), RootView(new QSplitter(this)) {
        RootView->setOrientation(Qt::Vertical);

        setLayout(new QGridLayout);
        layout()->setContentsMargins(0, 0, 0, 0);
        layout()->addWidget(RootView);
    }

    EditorWindow::Page::~Page() {}
/// ----------------------------------------------------------------

    EditorWindow::MongoConAndEditorPage::MongoConAndEditorPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        MongoDBConsole* const Console = new MongoDBConsole("mongosh", this);
        TreeEditor* const Editor = new TreeEditor;
        Console->AddAssociatedEditor(Editor);

        auto ShowMongoDBInWndTitle = [=, this]() { this->thisAtEditorWindow->UpdateWindowTitleWithSuffix("MongoDB Console"); };
        connect(Console, &MongoDBConsole::MouseDown, thisAtEditorWindow, ShowMongoDBInWndTitle);
        connect(Console->ExecuteButton, &QPushButton::clicked, thisAtEditorWindow, ShowMongoDBInWndTitle);
        connect(Console->CommandForm, &TextArea::MouseDown, thisAtEditorWindow, [=]() {
            ShowMongoDBInWndTitle();
            thisAtEditorWindow->UpdateFileTypeLabel("JavaScript");
            thisAtEditorWindow->UpdateCharsetLabel("UTF-8");
            });
        {
            auto UpdateStatusInfo = [=]() {
                ShowMongoDBInWndTitle();
                thisAtEditorWindow->UpdateFileTypeLabel("Plain Text");
                thisAtEditorWindow->UpdateCharsetLabel("Unicode");
            };
            connect(Console->URLForm, &TextField::MouseDown, thisAtEditorWindow, UpdateStatusInfo);
            connect(Console->mongoshCommandForm, &TextField::MouseDown, thisAtEditorWindow, UpdateStatusInfo);
        }
        {
            auto UpdateStatusInfo = [=]() {
                ShowMongoDBInWndTitle();
                thisAtEditorWindow->UpdateFileTypeLabel("MongoDB Extended JSON");
                thisAtEditorWindow->UpdateCharsetLabel("UTF-8");
            };
            connect(Editor, &TreeEditor::ShouldUpdateCharset, thisAtEditorWindow, UpdateStatusInfo);
            connect(Editor, &TreeEditor::ShouldUpdateFileType, thisAtEditorWindow, UpdateStatusInfo);
        }

        RootView->addWidget(Console);
        RootView->addWidget(Editor);
        RootView->setStretchFactor(0, 1);
        RootView->setStretchFactor(1, 4);
    }
/// ----------------------------------------------------------------

    EditorWindow::AnotherMongoConAndEditorPage::AnotherMongoConAndEditorPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        AnotherMongoDBConsole* const Console = new AnotherMongoDBConsole;
        TreeEditor* const Editor = new TreeEditor;
        Console->AddAssociatedEditor(Editor);

        RootView->addWidget(Console);
        RootView->addWidget(Editor);
        RootView->setStretchFactor(0, 1);
        RootView->setStretchFactor(1, 4);
    }
/// ----------------------------------------------------------------

    EditorWindow::EditorOnlyPage::EditorOnlyPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        TreeEditor* const Editor = new TreeEditor;

        connect(Editor, &TreeEditor::ShouldUpdatePathName, thisAtEditorWindow, &EditorWindow::UpdateWindowTitleWithPathName);
        connect(Editor, &TreeEditor::ShouldUpdateFileType, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateFileTypeLabel));
        connect(Editor, &TreeEditor::ShouldUpdateCharset, thisAtEditorWindow, qOverload<>(&EditorWindow::UpdateCharsetLabel));
        {
            auto UpdateStatusInfo = [=]() {
                thisAtEditorWindow->UpdateWindowTitleWithSuffix(Editor->GetPathName());
                thisAtEditorWindow->UpdateFileTypeLabel(Editor->GetFileType());
                thisAtEditorWindow->UpdateCharsetLabel(Editor->GetCharset());
            };
            connect(Editor->IntuitiveView, &TreeView::MouseDown, thisAtEditorWindow, UpdateStatusInfo);
            connect(Editor->RawView, &TextArea::MouseDown, thisAtEditorWindow, UpdateStatusInfo);
        };

        RootView->addWidget(Editor);
    }
/// ----------------------------------------------------------------

    EditorWindow::PythonInteractorPage::PythonInteractorPage(EditorWindow* const OuterInstance, QWidget* const Parent) : Page(OuterInstance, Parent) {
        auto* const PyInteractor = new PythonInteractor;
        auto ShowPyInteractornWndTitle = [=, this]() { this->thisAtEditorWindow->UpdateWindowTitleWithSuffix("Python Interactor"); };
        connect(PyInteractor->PyCommandForm, &TextField::MouseDown, thisAtEditorWindow, [=]() {
            ShowPyInteractornWndTitle();
            OuterInstance->UpdateFileTypeLabel("Plain Text");
            OuterInstance->UpdateCharsetLabel("Unicode");
            });
        connect(PyInteractor->CodeArea, &TextArea::MouseDown, thisAtEditorWindow, [=]() {
            ShowPyInteractornWndTitle();
            OuterInstance->UpdateFileTypeLabel("Python");
            OuterInstance->UpdateCharsetLabel("Unicode");
            });
        connect(PyInteractor->ResultArea, &TextArea::MouseDown, thisAtEditorWindow, [=]() {
            ShowPyInteractornWndTitle();
            OuterInstance->UpdateFileTypeLabel("Plain Text");
            OuterInstance->UpdateCharsetLabel("<OS default charset>");
            });
        RootView->addWidget(PyInteractor);
    }
}
