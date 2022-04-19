#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QGridLayout>

#include "Editor.h"
#include "MongoDBConsole.h"

namespace WritingMaterialsManager {
    EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent),
                                                  UI(new Ui::EditorWindow),
                                                  RootView(new QSplitter(this)) {
        UI->setupUi(this);

        QTabWidget* const TabView = new QTabWidget;
        auto* const MDBCPage = new MongoDBConsoleAndEditorPage;
        auto* const EditorPage = new EditorOnlyPage;
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

/// ----------------------------------------------------------------

    EditorWindow::Page::Page(QWidget* const Parent) : RootView(new QSplitter(this)) {
        RootView->setOrientation(Qt::Vertical);

        setLayout(new QGridLayout);
        layout()->setContentsMargins(0, 0, 0, 0);
        layout()->addWidget(RootView);
    }

    EditorWindow::Page::~Page() {}

/// ----------------------------------------------------------------

    EditorWindow::MongoDBConsoleAndEditorPage::MongoDBConsoleAndEditorPage(QWidget* const Parent) : Page(Parent) {
        MongoDBConsole* const Console = new MongoDBConsole;
        Editor* const Editor = new class Editor;
        Console->AddAssociatedEditor(Editor);
        RootView->addWidget(Console);
        RootView->addWidget(Editor);
        RootView->setStretchFactor(0, 1);
        RootView->setStretchFactor(1, 4);
    }

    EditorWindow::EditorOnlyPage::EditorOnlyPage(QWidget* const Parent) : Page(Parent) {
        RootView->addWidget(new Editor);
    }
}
