#ifndef WRITING_MATERIALS_MANAGER_DATABASECONSOLE_H
#define WRITING_MATERIALS_MANAGER_DATABASECONSOLE_H

#include <set>

#include <QWidget>

#include "TreeEditor.h"

namespace WritingMaterialsManager {
    class ShellConsole : public QWidget {
    Q_OBJECT
    public:
        explicit ShellConsole(QWidget* const Parent = nullptr);
        ~ShellConsole() = default;

        void AddAssociatedEditor(TreeEditor* const Editor);
        void RemoveAssociatedEditor(TreeEditor* const Editor);
        void ClearAssociatedEditor();
    public slots:
        void SetTextForAssociatedEditors(const QString& Text);
        void AppendTextForAssociatedEditors(const QString& Text);
        virtual void ArrangeContentViewForAssociatedEditors();
    protected:
        std::set<TreeEditor*> AssociatedEditors{};
    };
}

#endif //WMM_DATABASECONSOLE_H
