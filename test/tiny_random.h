#ifndef WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
#define WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H

#include <chrono>
#include <random>

namespace tiny_random {
    namespace {
        std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<uintmax_t> max_uniform_uint_dist(0, UINTMAX_MAX);
        std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
    }

    namespace number {
        template<class T> T integer() {
            static_assert(std::is_integral_v<T>, "Only built-in integral types are allowed.");
            if constexpr (std::is_signed_v<T>) return max_uniform_int_dist(random_engine);
            else return max_uniform_uint_dist(random_engine);
        }

        // return a random integer between [a, b], b - a <= max(T)
        template<class T> T integer(const T a, const T b) {
            static_assert(std::is_integral_v<T>, "Only built-in integral types are allowed.");
            const T L = b - a;
            if constexpr (std::is_signed_v<T>) {
                const T x = max_uniform_int_dist(random_engine);
                return a + (x % L - INTMAX_MIN % L) % L;
            }
            else {
                const T x = max_uniform_uint_dist(random_engine);
                return a + x % L;
            }
        }
    }

    namespace string {
        namespace {
            constexpr char hex[] = "0123456789ABCDEF";
            constexpr char latin[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            constexpr char alnum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            constexpr char punct[] = "!\"#$%&'()*+,-./;:<=>?@[\\]^_`{|}~";
        }

        template<class T> constexpr bool is_sbc_type_v = std::is_same_v<T, char> || std::is_same_v<signed char> || std::is_same_v<unsigned char>;

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> dec_digit() {
            return number::integer('0', '9');
        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> hex_digit() {
            return hex[number::integer(0, 15)];
        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> ucase_Latin() {
            return number::integer('A', 'Z');
        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> lcase_Latin() {
            return number::integer('a', 'z');
        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> Latin() {
            return latin[number::integer(0, 26 + 26 - 1)];
        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> digit_and_Latin() {
            return alnum[number::integer(0, 10 + 26 + 26 - 1)];
        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> printable_ASCII() {
            return number::integer(0x20, 0x7E);
        }
    }

    namespace Misc {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
