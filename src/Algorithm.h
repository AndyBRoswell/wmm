#ifndef WRITING_MATERIALS_MANAGER_ALGORITHM_H
#define WRITING_MATERIALS_MANAGER_ALGORITHM_H

#include <QCryptographicHash>
#include <QString>

namespace WritingMaterialsManager {
    struct CaseInsensitiveHasher {
        static constexpr auto DefaultHashAlgorithm = QCryptographicHash::Blake2b_160;
        
        template<class T> consteval static bool is_UTF_8_compatible_charset_f() {
            return std::is_same_v<T, QByteArrayView> || std::is_same_v<T, QLatin1StringView> || std::is_same_v<T, QUtf8StringView>
                // types which may have implicit conversions to the types above
                || std::is_same_v<std::remove_cvref_t<T>, const char*> || std::is_same_v<std::remove_cvref_t<T>, const char8_t*>;
        }
        template<class T> struct is_UTF_8_compatible_charset : std::integral_constant<bool, is_UTF_8_compatible_charset_f<T>()> {};;
        template<class T> static constexpr bool is_UTF_8_compatible_charset_v = is_UTF_8_compatible_charset<T>::value;
        
        size_t operator()(const QByteArray& Str) const noexcept;
        size_t operator()(const QString& Str) const noexcept;
        size_t operator()(const QAnyStringView Str) const noexcept; // Qt recommends pass string views by value
        size_t operator()(const QStringView Str) const noexcept;
        template<class T = QByteArrayView> typename std::enable_if_t<is_UTF_8_compatible_charset_v<T>, size_t> operator()(const T Str) const noexcept {
            if constexpr (std::is_class_v<T>) { return this->operator()(QByteArray::fromRawData(Str.data(), Str.size())); } // data() returns const ptr
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, const char*>) { return this->operator()(QByteArrayView(Str)); }
            else { return this->operator()(QUtf8StringView(Str)); }
        }
    };

    struct CaseInsensitiveStringComparator {
        template<class T> consteval static bool supported_but_should_be_by_ref_f() {
            return std::is_same_v<T, QString> || std::is_same_v<T, QByteArray>;
        }
        template<class T> struct supported_but_should_be_by_ref : std::integral_constant<bool, supported_but_should_be_by_ref_f<T>()> {};
        template<class T> static constexpr bool supported_but_should_be_by_ref_v = supported_but_should_be_by_ref<T>::value;
        
        template<class T> consteval static bool supported_but_only_member_compare_f() {
            return std::is_same_v<T, QByteArrayView> || std::is_same_v<T, QLatin1StringView> || std::is_same_v<T, QStringView>
                // types which may have implicit conversions to the types above; hint by qt
                || std::is_same_v<std::remove_cvref_t<T>, const char*> || std::is_same_v<std::remove_cvref_t<T>, const char8_t*>
                || std::is_same_v<std::remove_cvref_t<T>, const char16_t*> || std::is_same_v<std::remove_cvref_t<T>, const uint16_t*> || std::is_same_v<std::remove_cvref_t<T>, const QChar*>;
        }
        template<class T> struct supported_but_only_member_compare : std::integral_constant<bool, supported_but_only_member_compare_f<T>()> {};
        template<class T> static constexpr bool supported_but_only_member_compare_v = supported_but_only_member_compare<T>::value;
        
        bool operator()(const QAnyStringView LHS, const QAnyStringView RHS) const noexcept; // Qt recommends pass string views by value
        bool operator()(const QUtf8StringView LHS, const QUtf8StringView RHS) const noexcept;
        template<class T = QByteArrayView> typename std::enable_if_t<supported_but_only_member_compare_v<T>, bool> operator()(const T LHS, const T RHS) const noexcept {
            if constexpr (std::is_class_v<T>) { return LHS.compare(RHS, Qt::CaseInsensitive) == 0; }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, const char*>) {
                return QByteArrayView(LHS).compare(QByteArrayView(RHS), Qt::CaseInsensitive) == 0;
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, const char8_t*>) {
                return this->operator()(QUtf8StringView(LHS), QUtf8StringView(RHS));
            }
            else { return QStringView(LHS).compare(QStringView(RHS), Qt::CaseInsensitive) == 0; }
        }
        template<class T = QByteArray> typename std::enable_if_t<supported_but_should_be_by_ref_v<T>, bool> operator()(const T& LHS, const T& RHS) const noexcept {
            return LHS.compare(RHS, Qt::CaseInsensitive) == 0;
        }
    };
}

#endif //WRITING_MATERIALS_MANAGER_ALGORITHM_H
