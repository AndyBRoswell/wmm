#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QSplitter>

namespace WritingMaterialsManager {
    namespace Ui {
        class EditorWindow;
    }

    class EditorWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit EditorWindow(QWidget* Parent = nullptr);
        ~EditorWindow();

    private:
        class Page : public QWidget {
        public:
            std::shared_ptr<QSplitter> SplitView;

            Page(QWidget* const Parent = nullptr);
            ~Page();

            void AddSubWindow(QWidget* const Wnd);
        };

        Ui::EditorWindow* UI;
    };
}

#endif // EDITOR_H
