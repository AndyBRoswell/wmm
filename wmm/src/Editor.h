#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

namespace WritingMaterialsManager {
    namespace Ui {
        class Editor;
    }

    class Editor : public QMainWindow {
    Q_OBJECT

    public:
        explicit Editor(QWidget* parent = nullptr);
        ~Editor();

    private:
        class Page: public QWidget {
        public:
            Page(QWidget* const Parent = nullptr);
            ~Page();
        };

        Ui::Editor* UI;
    };
}

#endif // EDITOR_H
