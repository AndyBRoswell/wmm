#ifndef WRITING_MATERIALS_MANAGER_DATABASECONSOLE_H
#define WRITING_MATERIALS_MANAGER_DATABASECONSOLE_H

#include <set>

#include <QWidget>

#include "TreeEditor.h"

namespace WritingMaterialsManager {
    class DatabaseConsole : public QWidget {
    Q_OBJECT
    public:
        explicit DatabaseConsole(QWidget* const Parent = nullptr);
        ~DatabaseConsole() = default;

        void AddAssociatedEditor(TreeEditor* const Editor); // this console takes the ownership of the editor
        void RemoveAssociatedEditor(TreeEditor* const Editor);
        void ClearAssociatedEditor();
        size_t AssociatedEditorCount() const;
        void mousePressEvent(QMouseEvent* const E) override;
    signals:
        void MouseDown();
    public slots:
        void SetTextForAssociatedEditors(const QString& Text);
        void AppendTextForAssociatedEditors(const QString& Text);
        void RefreshAssociatedEditors();
        virtual void ArrangeContentViewForAssociatedEditors();
    protected:
        std::set<TreeEditor*> AssociatedEditors{};
    };
}

#endif //WMM_DATABASECONSOLE_H
