#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QGridLayout>

namespace WritingMaterialsManager {
/// EditorWindow

    EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), UI(new Ui::EditorWindow), RootView(new QSplitter(this)) {
        UI->setupUi(this);

        // preparation
        QTabWidget* TabView = new QTabWidget;
        TabView->addTab(new MongoDBConsoleAndEditorPage, "MongoDB Console");
        TabView->addTab(new EditorOnlyPage, "Editor Only");
        RootView->addWidget(TabView);

        setWindowTitle(tr("编辑器"));
        centralWidget()->setLayout(new QGridLayout);
        centralWidget()->layout()->addWidget(RootView);
        show();
    }

    EditorWindow::~EditorWindow() {
        delete UI;
    }

/// EditorWindow::Page

    EditorWindow::Page::Page(QWidget* const Parent) : RootView(new QSplitter(this)) {
        RootView->setOrientation(Qt::Vertical);

        setLayout(new QGridLayout);
        layout()->addWidget(RootView);
    }

    EditorWindow::Page::~Page() {}

/// EditorWindow::DefaultPage

    EditorWindow::MongoDBConsoleAndEditorPage::MongoDBConsoleAndEditorPage(QWidget* const Parent) : Page(Parent) {

    }

    EditorWindow::EditorOnlyPage::EditorOnlyPage(QWidget* const Parent) : Page(Parent) {

    }
}
