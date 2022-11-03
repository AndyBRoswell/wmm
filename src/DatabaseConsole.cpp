#include "DatabaseConsole.h"

namespace WritingMaterialsManager {
    DatabaseConsole::DatabaseConsole(QWidget* const Parent) : QWidget(Parent) {}

    void DatabaseConsole::AddAssociatedEditor(TreeEditor* const Editor) { AssociatedEditors.emplace(Editor); }

    void DatabaseConsole::RemoveAssociatedEditor(TreeEditor* const Editor) {
        auto TargetIterator = AssociatedEditors.find(Editor);
        AssociatedEditors.erase(TargetIterator);
        delete Editor;
    }

    void DatabaseConsole::ClearAssociatedEditor() {
        for (auto* Editor: AssociatedEditors) { delete Editor; }
        AssociatedEditors.clear();
    }

    size_t DatabaseConsole::AssociatedEditorCount() { return AssociatedEditors.size(); }

    void DatabaseConsole::SetTextForAssociatedEditors(const QString& Text) {
        for (auto& Editor: AssociatedEditors) { Editor->SetText(Text); }
    }

    void DatabaseConsole::AppendTextForAssociatedEditors(const QString& Text) {
        for (auto& Editor: AssociatedEditors) { Editor->AppendText(Text); }
    }

    void DatabaseConsole::RefreshAssociatedEditors() {
        for (auto& Editor: AssociatedEditors) { Editor->update(); }
    }

    void DatabaseConsole::ArrangeContentViewForAssociatedEditors() {
        for (auto& Editor: AssociatedEditors) { Editor->ArrangeContentView(); }
    }

    void DatabaseConsole::mousePressEvent(QMouseEvent* const E) {
        emit MouseDown();
    }
}
