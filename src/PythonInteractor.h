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
        explicit PythonAccessor(const QString& PythonCommand = "python");

        void SendResult();
    public slots:
        void ChangeInterpreter(const QString& PythonCommand = "python");
        void Execute(const QString& Code);
    signals:
        void MoreResult(const QString& Result);
        void NoMoreResult();
    private:
        std::shared_ptr<QProcess> PythonProcess;
        QString PythonCommand;
    };

    class PythonInteractor : public QWidget {
    Q_OBJECT
    public:
        TextField* const PyCommandForm;
        QPushButton* const ExecuteButton;
        TextArea* const CodeArea;
        TextArea* const ResultArea;

        explicit PythonInteractor(const QString& PythonCommand = "py/venv/latest/scripts/python", QWidget* const Parent = nullptr);
        ~PythonInteractor();

        void ExecuteCode();
    signals:
        void NewCode(const QString& Code);
    private:
        PythonAccessor PyAccessor;
        QThread PyAccessThread;
    };
}

#endif //WRITING_MATERIALS_MANAGER_PYTHONCONSOLE_H
