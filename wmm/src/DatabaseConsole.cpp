#include "DatabaseConsole.h"

namespace WritingMaterialsManager {
    DatabaseConsole::DatabaseConsole(QWidget* const Parent) : QWidget(Parent) {}

    void DatabaseConsole::AddAssociatedEditor(Editor* const Editor) { AssociatedEditors.emplace(Editor); }

    void DatabaseConsole::DeleteAssociatedEditor(Editor* const Editor) {
        auto TargetIterator = AssociatedEditors.find(Editor);
        delete *TargetIterator;
        AssociatedEditors.erase(TargetIterator);
    }

    void DatabaseConsole::SetTextForAssociatedEditors(const QString& Text) {
        for (auto& Editor: AssociatedEditors) { Editor->SetText(Text); }
    }
}
