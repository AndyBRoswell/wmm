#ifndef WRITING_MATERIALS_MANAGER_EDITOR_H
#define WRITING_MATERIALS_MANAGER_EDITOR_H

#include <QPlainTextEdit>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>

#include "QtTreeModel.h"

namespace WritingMaterialsManager {
    class Editor : public QWidget {
    Q_OBJECT

    public:
        QTabWidget* const TabView;
        QTreeView* const IntuitiveView;
        QPlainTextEdit* const RawView;

        explicit Editor(QWidget* const parent = nullptr, const std::shared_ptr<QtTreeModel>& TreeModel = nullptr);
        ~Editor();

        void SetText(const QString& Text = {});

    private:
        std::shared_ptr<QtTreeModel> TreeModel;
    };
} // namespace WritingMaterialsManager

#endif // WRITING_MATERIALS_MANAGER_EDITOR_H
