#include "TextHighlighter.h"

namespace WritingMaterialsManager {
    using TextHiliter = TextHighlighter;

    TextHiliter::TextHighlighter(QTextDocument
    * const TargetDoc):
    QSyntaxHighlighter(TargetDoc) {}
}
