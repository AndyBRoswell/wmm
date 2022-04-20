#include "PythonInteractor.h"

#include <QSplitter>
#include <QVBoxLayout>

namespace WritingMaterialsManager {
    PythonInteractor::PythonInteractor(const QString& PythonCommand, QWidget* const Parent) : QWidget(Parent),
                                                                                              PyCommandForm(new TextField(PythonCommand)),
                                                                                              ExecuteButton(new QPushButton("▶")),
                                                                                              CodeArea(new TextArea),
                                                                                              ResultArea(new TextArea) {
        setLayout(new QVBoxLayout);
        QWidget* const ControlArea = new QWidget;
        ControlArea->setLayout(new QHBoxLayout);
        ControlArea->layout()->setContentsMargins(0, 0, 0, 0);
        ControlArea->layout()->setSpacing(2);
        ControlArea->layout()->addWidget(PyCommandForm);
        ControlArea->layout()->addWidget(ExecuteButton);

        QSplitter* const InputArea = new QSplitter;
        InputArea->setOrientation(Qt::Vertical);
        InputArea->setContentsMargins(0, 0, 0, 0);
        InputArea->addWidget(CodeArea);
        InputArea->addWidget(ResultArea);
        InputArea->setStretchFactor(0, 0);
        InputArea->setStretchFactor(1, 4);

        layout()->setContentsMargins(0, 0, 0, 0);
        layout()->setSpacing(2);
        layout()->addWidget(ControlArea);
        layout()->addWidget(InputArea);
        static_cast<QVBoxLayout*>(layout())->setStretch(0, 0);
    }
}
