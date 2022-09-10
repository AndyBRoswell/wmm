#include "JSONHighlighter.h"

#include <mutex>

namespace WritingMaterialsManager {
    void JSONHighlighter::OneOffInit() {
        // set color of keywords
        KeywordFormat.setForeground(VisualStudioColorTheme.Keyword);
        for (const QString& KeywordPattern: KeywordPatterns) {
            HighlightRules.emplace_back(HighlightRule{ QRegularExpression(KeywordPattern), KeywordFormat });
        }

        // set color of numbers
        NumberFormat.setForeground(VisualStudioColorTheme.Number);
        HighlightRules.emplace_back(HighlightRule{ QRegularExpression(NumberPattern), NumberFormat });

        // set colors of strings
        KeyStringFormat.setForeground(VisualStudioColorTheme.KeyString);
        NonKeyStringFormat.setForeground(VisualStudioColorTheme.NonKeyString);
        EscapedStringFormat.setForeground(VisualStudioColorTheme.EscapedString);

        // set colors of errors
        ErrorFormat.setForeground(VisualStudioColorTheme.Error);
    }

    JSONHighlighter::JSONHighlighter(QTextDocument* const TargetDoc) : TextHighlighter(TargetDoc) {
        static std::once_flag StaticInitFlag;
        std::call_once(StaticInitFlag, OneOffInit);
    }

    void JSONHighlighter::highlightBlock(const QString& Text) {
        try {
            Highlight(Text);
        }
        catch (const std::exception& e) {
            qDebug() << "Exception at" << __FUNCTION__ << "when attempting to highlight JSON.";
            qDebug() << e.what();
        }
    }

    void JSONHighlighter::Highlight(const QString& Text) {
        using namespace std;

        // Match strings and escapes in strings. All intervals conform to the form [a, b).
        vector<qsizetype> StrBeginPos, StrEndPos;
        vector<bool> IsKey;
        vector<pair<qsizetype, qsizetype>> NonEscapedRange;
        vector<pair<qsizetype, qsizetype>> EscapedRange;
        vector<bool> EscapeError;
        pair<qsizetype, qsizetype> CurrentString;
        pair<qsizetype, qsizetype> CurrentEscape;
        enum class StrDFAState : size_t {
            Start = 0,
            NonEscaped = 1,
            Escaped = 2,
        };
        StrDFAState State = StrDFAState::Start;

        for (qsizetype i = 0; i < Text.length(); ++i) {
            switch (State) {
            case StrDFAState::Start:
                switch (Text[i].unicode()) {
                case u'\"':
                    CurrentString.first = i;
                    StrBeginPos.emplace_back(i);
                    State = StrDFAState::NonEscaped;
                    continue;
                case u':':
                    if (IsKey.empty() == false) *IsKey.rbegin() = true;
                    continue;
                }
            case StrDFAState::NonEscaped:
                switch (Text[i].unicode()) {
                case u'\"':
                    CurrentString.second = i + 1;
                    NonEscapedRange.emplace_back(CurrentString);
                    StrEndPos.emplace_back(i + 1);
                    IsKey.emplace_back(false);
                    State = StrDFAState::Start;
                    continue;
                case u'\\':
                    CurrentEscape.first = i;
                    State = StrDFAState::Escaped;
                    continue;
                default:
                    continue;
                }
            case StrDFAState::Escaped:
                switch (Text[i].unicode()) { // \", \\, \/, \b, \f, \n, \r, \t, \uhhhh
                case u'\"': case u'\\': case u'/': case u'b': case u'f': case u'n': case u'r': case u't':
                    CurrentEscape.second = i + 1;
                    EscapedRange.emplace_back(CurrentEscape);
                    EscapeError.emplace_back(false);
                    State = StrDFAState::NonEscaped;
                    continue;
                case u'u':
                    if (Text.length() - i <= 5) { // unexpected EOF
                        CurrentEscape.second = Text.length();
                        EscapedRange.emplace_back(CurrentEscape);
                        EscapeError.emplace_back(true);
                        continue;
                    }
                    else { // '\uhhhh"'
                        bool unknown_unicode = false;
                        for (qsizetype j = i + 1; j <= i + 4; ++j) {
                            if (isxdigit(Text[j].unicode()) == false) {
                                CurrentEscape.second = j;
                                EscapedRange.emplace_back(CurrentEscape);
                                EscapeError.emplace_back(true);
                                unknown_unicode = true;
                                State = StrDFAState::NonEscaped;
                                i = j;
                                break;
                            }
                        }
                        if (unknown_unicode == false) {
                            CurrentEscape.second = i + 5;
                            EscapedRange.emplace_back(CurrentEscape);
                            EscapeError.emplace_back(false);
                            State = StrDFAState::NonEscaped;
                            i += 4;
                            continue;
                        }
                        continue;
                    }
                default: // wrong escape
                    CurrentEscape.second = i + 1;
                    EscapedRange.emplace_back(CurrentEscape);
                    EscapeError.emplace_back(true);
                    State = StrDFAState::NonEscaped;
                    continue;
                }
            }
        }

        // highlight strings
        for (const auto& Range: NonEscapedRange) {
            const auto ptr = lower_bound(StrBeginPos.cbegin(), StrBeginPos.cend(), Range.first);
            const auto off = ptr - StrBeginPos.cbegin();
            switch (IsKey[off]) {
            case true:
                setFormat(Range.first, Range.second - Range.first, KeyStringFormat);
                continue;
            case false:
                setFormat(Range.first, Range.second - Range.first, NonKeyStringFormat);
                continue;
            }
        }
        for (size_t i = 0; i < EscapedRange.size(); ++i) {
            switch (EscapeError[i]) {
            case true:
                setFormat(EscapedRange[i].first, EscapedRange[i].second - EscapedRange[i].first, ErrorFormat);
                continue;
            case false:
                setFormat(EscapedRange[i].first, EscapedRange[i].second - EscapedRange[i].first, EscapedStringFormat);
                continue;
            }
        }

        // match and highlight keywords and numbers in non-string parts
        QStringView TextRef(Text);
        if (StrBeginPos.empty()) {
            for (const auto& HighlightRule: HighlightRules) {
                QRegularExpressionMatchIterator MatchIterator = HighlightRule.Pattern.globalMatch(Text);
                while (MatchIterator.hasNext()) {
                    QRegularExpressionMatch Match = MatchIterator.next();
                    setFormat(Match.capturedStart(), Match.capturedLength(), HighlightRule.Format);
                }
            }
        }
        else {
            for (const auto& HighlightRule: HighlightRules) {
                {
                    QRegularExpressionMatchIterator MatchIterator = HighlightRule.Pattern.globalMatch(TextRef.sliced(0, StrBeginPos[0]));
                    while (MatchIterator.hasNext()) {
                        QRegularExpressionMatch Match = MatchIterator.next();
                        setFormat(Match.capturedStart(), Match.capturedLength(), HighlightRule.Format);
                    }
                }
                for (size_t i = 0; i < StrEndPos.size() - 1; ++i) {
                    QRegularExpressionMatchIterator MatchIterator = HighlightRule.Pattern.globalMatch(TextRef.sliced(StrEndPos[i], StrBeginPos[i + 1] - StrEndPos[i]));
                    while (MatchIterator.hasNext()) {
                        QRegularExpressionMatch Match = MatchIterator.next();
                        setFormat(StrEndPos[i] + Match.capturedStart(), Match.capturedLength(), HighlightRule.Format);
                    }
                }
                {
                    QRegularExpressionMatchIterator MatchIterator = HighlightRule.Pattern.globalMatch(TextRef.sliced(StrEndPos[StrEndPos.size() - 1]));
                    while (MatchIterator.hasNext()) {
                        QRegularExpressionMatch Match = MatchIterator.next();
                        setFormat(StrEndPos[StrEndPos.size() - 1] + Match.capturedStart(), Match.capturedLength(), HighlightRule.Format);
                    }
                }
            }
        }
    }
}
