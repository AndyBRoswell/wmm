#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QGridLayout>

#include "Editor.h"
#include "MongoDBConsole.h"

namespace WritingMaterialsManager {
    EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), UI(new Ui::EditorWindow), RootView(new QSplitter(this)) {
        UI->setupUi(this);

        // preparation
        QTabWidget* const TabView = new QTabWidget;
        auto* const MDBCPage = new MongoDBConsoleAndEditorPage;
        MDBCPage->RootView->addWidget(new MongoDBConsole);
        MDBCPage->RootView->addWidget(new Editor);
        auto* const EditorPage = new EditorOnlyPage;
        EditorPage->RootView->addWidget(new Editor);
        TabView->addTab(MDBCPage, "MongoDB Console");
        TabView->addTab(EditorPage, "Editor Only");
        RootView->addWidget(new QWidget);

        setWindowTitle(tr("编辑器"));
        centralWidget()->setLayout(new QGridLayout);
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
        layout()->addWidget(RootView);
    }

    EditorWindow::Page::~Page() {}

/// ----------------------------------------------------------------

    EditorWindow::MongoDBConsoleAndEditorPage::MongoDBConsoleAndEditorPage(QWidget* const Parent) : Page(Parent) {}

    EditorWindow::EditorOnlyPage::EditorOnlyPage(QWidget* const Parent) : Page(Parent) {}
}
