#ifndef WRITING_MATERIALS_MANAGER_DATABASECONSOLE_H
#define WRITING_MATERIALS_MANAGER_DATABASECONSOLE_H

#include <set>

#include <QWidget>

#include "Editor.h"

namespace WritingMaterialsManager {
    class DatabaseConsole : public QWidget {
    Q_OBJECT
    public:
        explicit DatabaseConsole(QWidget* const Parent = nullptr);

        void AddAssociatedEditor(Editor* const Editor);
        void DeleteAssociatedEditor(Editor* const Editor);
        void ClearAssociatedEditor();
    public slots:
        void SetTextForAssociatedEditors(const QString& Text);
    private:
        std::set<Editor*> AssociatedEditors{};
    };
}

#endif //WMM_DATABASECONSOLE_H
