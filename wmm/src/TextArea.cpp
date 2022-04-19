#include "TextArea.h"

namespace WritingMaterialsManager {
    void TextField::mousePressEvent(QMouseEvent* E) {
        QLineEdit::mousePressEvent(E);
        emit MouseDown();
    }

    void TextArea::mousePressEvent(QMouseEvent* E) {
        QPlainTextEdit::mousePressEvent(E);
        emit MouseDown();
    }
}
