#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QGridLayout>

using namespace WritingMaterialsManager;

/// EditorWindow

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), UI(new Ui::EditorWindow) {
    UI->setupUi(this);

    // preparation
    setWindowTitle(tr("编辑器"));

    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(UI->EditorTab);
}

EditorWindow::~EditorWindow() {
    delete UI;
}

/// EditorWindow::Page

EditorWindow::Page::Page(QWidget* const Parent): SplitView(new QSplitter(this)) {
    SplitView->setOrientation(Qt::Vertical);

    setLayout(new QGridLayout);
    layout()->addWidget(SplitView.get());
    show();
}

EditorWindow::Page::~Page() {}

void EditorWindow::Page::AddSubWindow(QWidget* const Wnd) { SplitView->addWidget(Wnd); }
