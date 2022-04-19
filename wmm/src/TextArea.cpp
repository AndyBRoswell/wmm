#include "TextArea.h"

namespace WritingMaterialsManager {
    void TextArea::mousePressEvent(QMouseEvent* E) {
        QPlainTextEdit::mousePressEvent(E);
        emit MouseDown();
    }
}
