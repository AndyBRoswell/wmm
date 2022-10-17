#ifndef WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
#define WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H

#include <chrono>
#include <cinttypes>
#include <iostream>
#include <random>
#include <string>

namespace tiny_random {
    namespace {
        std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<uintmax_t> max_uniform_uint_dist(0, UINTMAX_MAX);
        std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
    }

    namespace number {
        template<class T> constexpr T mod(const T& N, const T& D) {
            const T r = N % D;
            if (D > 0) { return r >= 0 ? r : r + D; }
            else { return r <= 0 ? r : r + D; }
        }

        template<class T> T integer() {
            static_assert(std::is_integral_v<T>, "Only built-in integral types are allowed.");
            if constexpr (std::is_signed_v<T>) return max_uniform_int_dist(random_engine);
            else return max_uniform_uint_dist(random_engine);
        }

        // return a random integer between [a, b], b - a <= max(T)
        template<class T> T integer(const T a, const T b) {
            static_assert(std::is_integral_v<T>, "Only built-in integral types are allowed.");
            const T L = b - a + 1;
            if constexpr (std::is_signed_v<T>) {
                const T x = max_uniform_int_dist(random_engine);
                return a + mod(static_cast<intmax_t>(mod(x, L)) - mod(INTMAX_MIN, static_cast<intmax_t>(L)), static_cast<intmax_t>(L));
            }
            else {
                const T x = max_uniform_uint_dist(random_engine);
                return a + x % L;
            }
        }
    }

    namespace chr {
        namespace {
            constexpr char hex[] = "0123456789ABCDEF";
            constexpr char lhex[] = "0123456789abcdef";
            constexpr char latin[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            constexpr char ualnum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            constexpr char lalnum[] = "0123456789abcdefghijklmnopqrstuvwxyz";
            constexpr char alnum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            constexpr char punct[] = "!\"#$%&'()*+,-./;:<=>?@[\\]^_`{|}~";
        }

        enum class ASCII_char_type {
            dec, hex, lhex, ucase, lcase, alpha, ualnum, lalnum, alnum, punct, printable,
        };

        template<class T> constexpr bool is_sbc_type_v = std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>;

        template<class T = char> typename std::enable_if_t<is_sbc_type_v<T>, T> ASCII(const ASCII_char_type type = ASCII_char_type::printable) {
            using t = ASCII_char_type;
            switch (type) {
            case t::dec: return number::integer('0', '9');
            case t::hex: return hex[number::integer(0, 15)];
            case t::lhex: return lhex[number::integer(0, 15)];
            case t::ucase: return number::integer('A', 'Z');
            case t::lcase: return number::integer('a', 'z');
            case t::alpha: return latin[number::integer(0, 26 - 1)];
            case t::ualnum: return ualnum[number::integer(0, 10 + 26 - 1)];
            case t::lalnum: return lalnum[number::integer(0, 10 + 26 - 1)];
            case t::alnum: return alnum[number::integer(0, 10 + 26+26 - 1)];
            case t::punct: return punct[number::integer(0ull, sizeof(punct) - 1)]; // 0ull -> 0uz since C++23
            default: return number::integer(0x20, 0x7E);
            }
        }

        template<class T = char> typename std::basic_string<std::enable_if_t<is_sbc_type_v<T>, T>> ASCII_string(const size_t length, const ASCII_char_type type = ASCII_char_type::printable) {
            std::basic_string<T> s;
            for (size_t i = 0; i < length; ++i) { s.push_back(ASCII(type)); }
            return s;
        }
    }

    namespace Misc {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
