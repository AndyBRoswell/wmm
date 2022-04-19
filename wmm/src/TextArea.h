#ifndef WRITING_MATERIALS_MANAGER_TEXTAREA_H
#define WRITING_MATERIALS_MANAGER_TEXTAREA_H

#include <QLineEdit>
#include <QPlainTextEdit>

namespace WritingMaterialsManager {
    class TextField : public QLineEdit {
    Q_OBJECT
    public:
        using QLineEdit::QLineEdit;

        void mousePressEvent(QMouseEvent* E) override;
    signals:
        void MouseDown();
    };

    class TextArea : public QPlainTextEdit {
    Q_OBJECT
    public:
        using QPlainTextEdit::QPlainTextEdit;

        void mousePressEvent(QMouseEvent* E) override;
    signals:
        void MouseDown();
    };
}

#endif //WRITING_MATERIALS_MANAGER_TEXTAREA_H
