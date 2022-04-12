#include "Editor.h"
#include "ui_Editor.h"

using namespace WritingMaterialsManager;

Editor::Editor(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Editor) {
    ui->setupUi(this);
}

Editor::~Editor() {
    delete ui;
}
