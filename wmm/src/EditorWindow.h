#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

namespace WritingMaterialsManager {
    namespace Ui {
        class EditorWindow;
    }

    class EditorWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit EditorWindow(QWidget* parent = nullptr);
        ~EditorWindow();

    private:
        class Page: public QWidget {
        public:
            Page(QWidget* const Parent = nullptr);
            ~Page();
        };

        class MongoDBConsole: public QWidget {

        };

        Ui::EditorWindow* UI;
    };
}

#endif // EDITOR_H
