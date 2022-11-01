#include "PythonInteractor.h"

#include <QDir>
#include <QSplitter>
#include <QVBoxLayout>

namespace WritingMaterialsManager {
    PythonInteractor::PythonInteractor(const QString& PythonCommand, QWidget* const Parent) : 
        QWidget(Parent), PyAccessor(PythonCommand), PyCommandForm(new TextField(PythonCommand)), ExecuteButton(new QPushButton("▶")), CodeArea(new TextArea("import this\n")), ResultArea(new TextArea) {
        // event handlers regarding Python accessor
        PyAccessor.moveToThread(&PyAccessThread);
        connect(&PyAccessThread, &QThread::finished, &PyAccessor, &QObject::deleteLater);
        connect(ExecuteButton, &QPushButton::clicked, this, &PythonInteractor::ExecuteCode);
        connect(PyCommandForm, &TextField::textChanged, &PyAccessor, &PythonAccessor::ChangeInterpreter);
        connect(this, &PythonInteractor::NewCode, &PyAccessor, &PythonAccessor::Execute);
        connect(&PyAccessor, &PythonAccessor::MoreResult, ResultArea, &TextArea::appendPlainText);
        PyAccessThread.start();

        QWidget* const ControlArea = new QWidget; {
            ControlArea->setLayout(new QHBoxLayout);
            ControlArea->layout()->setContentsMargins(0, 0, 0, 0);
            ControlArea->layout()->setSpacing(2);
            ControlArea->layout()->addWidget(PyCommandForm);
            ControlArea->layout()->addWidget(ExecuteButton);
        }

        QSplitter* const InputArea = new QSplitter; {
            InputArea->setOrientation(Qt::Vertical);
            InputArea->setContentsMargins(0, 0, 0, 0);
            InputArea->addWidget(CodeArea);
            InputArea->addWidget(ResultArea);
            InputArea->setStretchFactor(0, 0);
            InputArea->setStretchFactor(1, 4);
        }

        QVBoxLayout* const MainLayout = new QVBoxLayout; {
            MainLayout->setContentsMargins(0, 0, 0, 0);
            MainLayout->setSpacing(2);
            MainLayout->addWidget(ControlArea);
            MainLayout->addWidget(InputArea);
            MainLayout->setStretch(0, 0);
        }
    }

    PythonInteractor::~PythonInteractor() {
        PyAccessThread.quit();
        PyAccessThread.wait();
    }

    void PythonInteractor::ExecuteCode() {
        ResultArea->setPlainText(""); // clear() will also delete the undo/redo history.
        emit NewCode(CodeArea->toPlainText());
    }
/// ----------------------------------------------------------------

    PythonAccessor::PythonAccessor(const QString& PythonCommand) : PythonCommand(PythonCommand) {}

    void PythonAccessor::ChangeInterpreter(const QString& PythonCommand) {
        this->PythonCommand = PythonCommand;
    }

    void PythonAccessor::SendResult() {
        using namespace std::chrono;
        using namespace std::chrono_literals;

//        time_point<high_resolution_clock> return_ends_time_point{};
//        while (return_ends_time_point.time_since_epoch().count() == 0 || high_resolution_clock::now() - return_ends_time_point <= 1s) {
//            const QByteArray Result = PythonProcess->readAllStandardOutput();
//            if (Result != "") {
//                emit MoreResult(Result);
//                qDebug() << Result;
//            }
//            else if (return_ends_time_point.time_since_epoch().count() == 0) return_ends_time_point = high_resolution_clock::now();
//        }
        PythonProcess->waitForFinished(-1);
        const QByteArray Result = PythonProcess->readAllStandardOutput();
//        qDebug() << Result;
        emit MoreResult(QString::fromLocal8Bit(Result));
        qDebug() << "No more return from Python.";
        emit NoMoreResult();
    }

    void PythonAccessor::Execute(const QString& Code) {
        PythonProcess = std::make_shared<QProcess>();
        PythonProcess->start(PythonCommand, { "-c", Code });
        PythonProcess->waitForReadyRead(-1);
        SendResult();
        PythonProcess = nullptr;
    }
}
