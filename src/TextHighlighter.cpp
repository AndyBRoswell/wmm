#include "TextHighlighter.h"

namespace WritingMaterialsManager {
    TextHighlighter::TextHighlighter(QTextDocument* const TargetDoc) : QSyntaxHighlighter(TargetDoc) {}
}
