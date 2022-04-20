#ifndef WRITING_MATERIALS_MANAGER_PYTHONCONSOLE_H
#define WRITING_MATERIALS_MANAGER_PYTHONCONSOLE_H

#include <QPushButton>

#include "TextArea.h"

namespace WritingMaterialsManager {
    class PythonInteractor : public QWidget {
    Q_OBJECT
    public:
        TextField* PyCommandForm;
        QPushButton* ExecuteButton;
        TextArea* CodeArea;
        TextArea* ResultArea;

        explicit PythonInteractor(const QString& PythonCommand = "py/venv/3.8/script/python", QWidget* const Parent = nullptr);
    };
}

#endif //WRITING_MATERIALS_MANAGER_PYTHONCONSOLE_H
