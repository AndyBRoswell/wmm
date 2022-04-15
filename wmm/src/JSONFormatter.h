#ifndef WRITING_MATERIALS_MANAGER_JSONFORMATTER_H
#define WRITING_MATERIALS_MANAGER_JSONFORMATTER_H

#include "TextFormatter.h"

namespace WritingMaterialsManager {
    class JSONFormatter : public TextFormatter {
    public:
        void Format(QString& Text) override;
    };
}

#endif //WRITING_MATERIALS_MANAGER_JSONFORMATTER_H
