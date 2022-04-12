#include "Editor.h"
#include "ui_Editor.h"

#include <QGridLayout>

using namespace WritingMaterialsManager;

Editor::Editor(QWidget* parent) : QMainWindow(parent), UI(new Ui::Editor) {
    UI->setupUi(this);

    // preparation
    setWindowTitle(tr("编辑器"));

    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(UI->EditorTab);
}

Editor::~Editor() {
    delete UI;
}
