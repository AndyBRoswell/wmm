#ifndef WRITING_MATERIALS_MANAGER_TEXTFORMATTER_H
#define WRITING_MATERIALS_MANAGER_TEXTFORMATTER_H

#include <QString>

namespace WritingMaterialsManager {
    class TextFormatter {
    public:
        virtual void Format(QString& Text) = 0;
    };
}

#endif //WRITING_MATERIALS_MANAGER_TEXTFORMATTER_H
