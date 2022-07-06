#ifndef WRITING_MATERIALS_MANAGER_JSONHIGHLIGHTER_H
#define WRITING_MATERIALS_MANAGER_JSONHIGHLIGHTER_H

#include <vector>

#include <QRegularExpression>

#include "TextHighlighter.h"

namespace WritingMaterialsManager {
    class JSONHighlighter : public TextHighlighter {
    public:
        struct ColorTheme {
            QBrush Default;
            QBrush Keyword;
            QBrush Number;
            QBrush KeyString;
            QBrush NonKeyString;
            QBrush EscapedString;
            QBrush Error;
        };

        inline static const ColorTheme VisualStudioColorTheme{
            QColor(212, 212, 212),
            QColor(86, 156, 214),
            QColor(181, 206, 168),
            QColor(156, 220, 254),
            QColor(206, 145, 120),
            QColor(215, 186, 125),
            QColor(244, 71, 71)
        };

        explicit JSONHighlighter(QTextDocument* const TargetDoc = nullptr);

        void Highlight(const QString& Text = "") override;
    protected:
        void highlightBlock(const QString& Text) override;

    private:
        struct HighlightRule {
            QRegularExpression Pattern;
            QTextCharFormat Format;
        };

        inline static const QString KeywordPatterns[] = { R"(\btrue\b)", R"(\bfalse\b)", R"(\bnull\b)" };
/*
 * general integers: 123456, 0123, +5, -2, ++++987, ----456, +-+-+--12
 * general floats: 0.123, .1234, 000.2345, 98765.4321, +1.23, -2.56, ++3.1, ---6.0, +-7.7
 * scientific notation: 1.2345E22, 45.678e+59, 0.998e-65
 */
        inline static const QString NumberPattern{ R"(\b[+-]*\d*\.?\d*([Ee]?[+-]*\d+)?\b)" };

        inline static QTextCharFormat KeywordFormat;
        inline static QTextCharFormat NumberFormat;
        inline static QTextCharFormat KeyStringFormat;
        inline static QTextCharFormat NonKeyStringFormat;
        inline static QTextCharFormat EscapedStringFormat;
        inline static QTextCharFormat ErrorFormat;

        inline static std::vector<HighlightRule> HighlightRules;

        static void OneOffInit();
    };
}

#endif //WRITING_MATERIALS_MANAGER_JSONHIGHLIGHTER_H
