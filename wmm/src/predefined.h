#ifndef WRITING_ASSISTANT_PREDEFINED_H
#define WRITING_ASSISTANT_PREDEFINED_H

// In order to avoid using 32-bit size types for indexing strings and arrays, define this preprocessor symbol and provide the type rapidjson::SizeType before including RapidJSON
#define RAPIDJSON_NO_SIZETYPEDEFINE
namespace rapidjson {
    using SizeType = unsigned long long;
}

#endif //WRITING_ASSISTANT_PREDEFINED_H
