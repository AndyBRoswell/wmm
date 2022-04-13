#ifndef WRITING_MATERIALS_MANAGER_EDITOR_H
#define WRITING_MATERIALS_MANAGER_EDITOR_H

#include <QWidget>

namespace WritingMaterialsManager {
    class Editor : public QWidget {
    Q_OBJECT
    public:
        explicit Editor(QWidget* parent = nullptr);

    signals:

    };
} // namespace WritingMaterialsManager

#endif // WRITING_MATERIALS_MANAGER_EDITOR_H
