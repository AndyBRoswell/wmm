#ifndef WRITING_MATERIALS_MANAGER_PREDEFINED_H
#define WRITING_MATERIALS_MANAGER_PREDEFINED_H

#define __ON_WINDOWS__ defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)

// In order to avoid using 32-bit size types for indexing strings and arrays, define this preprocessor symbol and provide the type rapidjson::SizeType before including RapidJSON
#define RAPIDJSON_NO_SIZETYPEDEFINE
namespace rapidjson {
    using SizeType = unsigned long long;
}

#endif //WRITING_MATERIALS_MANAGER_PREDEFINED_H
