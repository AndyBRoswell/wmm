#ifndef WRITING_MATERIALS_MANAGER_ALGORITHM_H
#define WRITING_MATERIALS_MANAGER_ALGORITHM_H

#include <QCryptographicHash>
#include <QString>

namespace WritingMaterialsManager {
    struct CaseInsensitiveHasher {
        static constexpr auto DefaultHashAlgorithm = QCryptographicHash::Blake2b_160;
        
        template<class T> consteval static bool is_UTF_8_compatible_charset_f() {
            return std::is_same_v<T, QByteArrayView> || std::is_same_v<T, QLatin1StringView> || std::is_same_v<T, QUtf8StringView>
                // types with implicit conversions to the types above
                || std::is_same_v<T, std::remove_cvref_t<char*>> || std::is_same_v<T, std::remove_cvref_t<char8_t>>;
        }
        template<class T> struct is_UTF_8_compatible_charset : std::integral_constant<bool, is_UTF_8_compatible_charset_f<T>()> {};;
        template<class T> static constexpr bool is_UTF_8_compatible_charset_v = is_UTF_8_compatible_charset<T>::value;
        
        size_t operator()(const QByteArray& Str) const noexcept;
        size_t operator()(const QString& Str) const noexcept;
        size_t operator()(const QAnyStringView Str) const noexcept;
        size_t operator()(const QStringView Str) const noexcept;
        template<class T = QByteArrayView> typename std::enable_if_t<is_UTF_8_compatible_charset_v<T>, size_t> operator()(const T Str) const noexcept {
            return this->operator()(QByteArray(Str.constData(), Str.size()));
        }
    };

    struct CaseInsensitiveStringComparator {
        template<class T> consteval static bool has_static_compare_f() {
            return std::is_same_v<T, QAnyStringView> 
                || std::is_same_v<T, QByteArrayView> || std::is_same_v<T, QLatin1StringView> || std::is_same_v<T, QStringView> || std::is_same_v<T, QUtf8StringView> 
                || std::is_same_v<T, QString>
                // types with implicit conversions to the types above; hint by qt
                || std::is_same_v<T, std::remove_cvref_t<char*>> || std::is_same_v<T, std::remove_cvref_t<char8_t*>>
                || std::is_same_v<T, std::remove_cvref_t<char16_t*>> || std::is_same_v<T, std::remove_cvref_t<unsigned short*>> || std::is_same_v<T, std::remove_cvref_t<QChar*>>;
        }
        template<class T> struct has_static_compare : std::integral_constant<bool, has_static_compare_f<T>()> {};
        template<class T> static constexpr bool has_static_compare_v = has_static_compare<T>::value;
        
        template<class T = QAnyStringView> typename std::enable_if_t<has_static_compare_v<T>, bool> operator()(const T LHS, const T RHS) const noexcept { // Qt recommends pass string views by value
            return T::compare(LHS, RHS, Qt::CaseInsensitive) == 0;
        }
        bool operator()(const QByteArray& LHS, const QByteArray& RHS) const noexcept {
            return LHS.compare(RHS, Qt::CaseInsensitive) == 0;
        }
    };
}

#endif //WRITING_MATERIALS_MANAGER_ALGORITHM_H
