#include "TextArea.h"

#include "global.h"

namespace WritingMaterialsManager {
    TextField::TextField(const QString& Text, QWidget* const Parent) : QLineEdit(Text, Parent) {
        setFont(DefaultCodeFont);
    }

    TextField::TextField(QWidget* const Parent) : QLineEdit(Parent) {
        setFont(DefaultCodeFont);
    }

    void TextField::mousePressEvent(QMouseEvent* E) {
        QLineEdit::mousePressEvent(E);
        emit MouseDown();
    }
    // ----------------------------------------------------------------

    TextArea::TextArea(const QString& Text, QWidget* const Parent) : QPlainTextEdit(Text, Parent) {
        setFont(DefaultCodeFont);
    }

    TextArea::TextArea(QWidget* const Parent) : QPlainTextEdit(Parent) {
        setFont(DefaultCodeFont);
    }

    void TextArea::mousePressEvent(QMouseEvent* const E) {
        QPlainTextEdit::mousePressEvent(E);
        emit MouseDown();
    }
}
