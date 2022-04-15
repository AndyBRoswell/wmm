#ifndef WRITING_MATERIALS_MANAGER_TEXTHIGHLIGHTER_H
#define WRITING_MATERIALS_MANAGER_TEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>

namespace WritingMaterialsManager {
    class TextHighlighter : public QSyntaxHighlighter {
    public:
        explicit TextHighlighter(QTextDocument* const TargetDoc = nullptr);

        virtual void Highlight(const QString& Text = "") = 0;
    };
}

#endif //WRITING_MATERIALS_MANAGER_TEXTHIGHLIGHTER_H
