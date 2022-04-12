#include "EditorWindow.h"
#include "ui_EditorWindow.h"

#include <QGridLayout>

using namespace WritingMaterialsManager;

/// EditorWindow

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent),UI(new Ui::EditorWindow) {
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

EditorWindow::Page::Page(QWidget* const Parent) {
    setLayout(new QGridLayout);
    show();
}

EditorWindow::Page::~Page() {}

void EditorWindow::Page::AddSubWindow(QWidget* const Wnd) { layout()->addWidget(Wnd); }
