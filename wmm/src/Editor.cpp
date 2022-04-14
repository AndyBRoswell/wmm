#include "Editor.h"

#include <QGridLayout>

namespace WritingMaterialsManager {
    Editor::Editor(QWidget* const parent, const std::shared_ptr<QtTreeModel>& TreeModel) : QWidget(parent),
                                                                                           TabView(new QTabWidget),
                                                                                           IntuitiveView(new QTreeView),
                                                                                           RawView(new QPlainTextEdit),
                                                                                           TreeModel(TreeModel) {
        IntuitiveView->setModel(TreeModel.get());
        TabView->addTab(IntuitiveView, tr("直观"));
        TabView->addTab(RawView, tr("原始"));

        setLayout(new QGridLayout);
        layout()->addWidget(TabView);
    }

    Editor::~Editor() {}

    void Editor::SetText(const QString& Text) { RawView->setPlainText(Text); }
} // namespace WritingMaterialsManager
