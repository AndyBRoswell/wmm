#ifndef WRITING_MATERIALS_MANAGER_TEXTAREA_H
#define WRITING_MATERIALS_MANAGER_TEXTAREA_H

#include <QLineEdit>
#include <QPlainTextEdit>

namespace WritingMaterialsManager {
    class TextField : public QLineEdit {
    Q_OBJECT
    public:
        using QLineEdit::QLineEdit; // Inheriting ctors: All base ctors are made visible to overload resolution when initializing this (derived) class

        explicit TextField(const QString& Text, QWidget* const Parent = nullptr);
        explicit TextField(QWidget* const Parent = nullptr);

        void mousePressEvent(QMouseEvent* const E) override; // mouse press event handler
    signals:
        void MouseDown();
    };

    class TextArea : public QPlainTextEdit {
    Q_OBJECT
    public:
        using QPlainTextEdit::QPlainTextEdit;

        explicit TextArea(const QString& Text, QWidget* const Parent = nullptr);
        explicit TextArea(QWidget* const Parent = nullptr);

        void mousePressEvent(QMouseEvent* E) override;
    signals:
        void MouseDown();
    };
}

#endif //WRITING_MATERIALS_MANAGER_TEXTAREA_H
