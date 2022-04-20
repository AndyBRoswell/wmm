#ifndef WRITING_MATERIALS_MANAGER_PYTHONCONSOLE_H
#define WRITING_MATERIALS_MANAGER_PYTHONCONSOLE_H

#include <QProcess>
#include <QPushButton>
#include <QThread>

#include "TextArea.h"

namespace WritingMaterialsManager {
    class PythonAccessor : public QObject {
    Q_OBJECT
    public:
        explicit PythonAccessor(const QString& PythonCommand);

        QString GetResult();
    public slots:
        void Execute(const QString& Code);
    signals:
        void MoreResult(const QString& Result);
        void NoMoreResult();
    private:
        std::shared_ptr<QProcess> PythonProcess;
    };

    class PythonInteractor : public QWidget {
    Q_OBJECT
    public:
        TextField* PyCommandForm;
        QPushButton* ExecuteButton;
        TextArea* CodeArea;
        TextArea* ResultArea;

        explicit PythonInteractor(const QString& PythonCommand = "py/venv/3.8/script/python", QWidget* const Parent = nullptr);
        ~PythonInteractor();

        void ExecuteCode();
    private:
        PythonAccessor PyAccessor;
        QThread PyAccessThread;
    };
}

#endif //WRITING_MATERIALS_MANAGER_PYTHONCONSOLE_H
