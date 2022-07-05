#include "TreeView.h"

namespace WritingMaterialsManager {
    void TreeView::mousePressEvent(QMouseEvent* E) {
        QTreeView::mousePressEvent(E);
        emit MouseDown();
    }
}
