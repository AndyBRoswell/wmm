#include "TreeView.h"

namespace WritingMaterialsManager {
    void TreeView::mousePressEvent(QMouseEvent* const E) {
        QTreeView::mousePressEvent(E);
        emit MouseDown();
    }
}
