#include "JSONFormatter.h"

#include <stdexcept>

#include <QDebug>

#include "rapidjson/reader.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stream.h"

namespace WritingMaterialsManager {
    // The parameter Text has QString type, mainly for the direct interaction with Widgets classes such as QPlainTextEdit
    void JSONFormatter::Format(QString& Text) {
        using namespace std;
        using namespace rapidjson;

        using InputEncoding = UTF16<char16_t>;
        using ParsingOutputEncoding = UTF8<>;
        using OutputEncoding = UTF16<char16_t>;

        GenericReader<InputEncoding, ParsingOutputEncoding> JSONReader;
        GenericStringStream<InputEncoding> JSONIStream(Text.toStdU16String().c_str());
        GenericStringBuffer<OutputEncoding> JSONOStream;
        PrettyWriter<GenericStringBuffer<OutputEncoding>, ParsingOutputEncoding, OutputEncoding> JSONWriter(JSONOStream);
        try {
            ParseResult ParseResult = JSONReader.Parse<ParseFlag::kParseFullPrecisionFlag>(JSONIStream, JSONWriter);
            if (ParseResult.IsError()) throw runtime_error(string("Exception at ") + __FUNCTION__ + ": Parsing ERROR.");
        }
        catch (const runtime_error& e) {
            qDebug() << e.what();
            return;
        }
        Text = QString::fromStdU16String(JSONOStream.GetString());
    }
}
