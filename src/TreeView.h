#ifndef WRITING_MATERIALS_MANAGER_TREEVIEW_H
#define WRITING_MATERIALS_MANAGER_TREEVIEW_H

#include <QTreeView>

namespace WritingMaterialsManager {
    class TreeView : public QTreeView {
    Q_OBJECT
    public:
        using QTreeView::QTreeView;

        void mousePressEvent(QMouseEvent* E) override;
    signals:
        void MouseDown();
    };
}

#endif //WRITING_MATERIALS_MANAGER_TREEVIEW_H
