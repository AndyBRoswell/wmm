#include "ShellConsole.h"

namespace WritingMaterialsManager {
    ShellConsole::ShellConsole(QWidget* const Parent) : QWidget(Parent) {}

    void ShellConsole::AddAssociatedEditor(TreeEditor* const Editor) { AssociatedEditors.emplace(Editor); }

    void ShellConsole::RemoveAssociatedEditor(TreeEditor* const Editor) {
        auto TargetIterator = AssociatedEditors.find(Editor);
        AssociatedEditors.erase(TargetIterator);
    }

    void ShellConsole::ClearAssociatedEditor() {
        for (auto* Editor: AssociatedEditors) { delete Editor; }
        AssociatedEditors.clear();
    }

    void ShellConsole::SetTextForAssociatedEditors(const QString& Text) {
        for (auto& Editor: AssociatedEditors) { Editor->SetText(Text); }
    }

    void ShellConsole::AppendTextForAssociatedEditors(const QString& Text) {
        for (auto& Editor: AssociatedEditors) { Editor->AppendText(Text); }
    }

    void ShellConsole::ArrangeContentViewForAssociatedEditors() {
        for (auto& Editor: AssociatedEditors) { Editor->ArrangeContentView(); }
    }
}
