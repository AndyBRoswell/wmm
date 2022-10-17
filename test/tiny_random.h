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
            const char dec_digit[] = "0123456789";
            const char hex_digit[] = "0123456789ABCDEF";
            const char upper_case_latin[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            const char lower_case_latin[] = "abcdefghijklmnopqrstuvwxyz";
            const char latin[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            const char digit_and_latin[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            const char punctuation[] = "!\"#$%&'()*+,-./;:<=>?@[\]^_`{|}~";
            std::uniform_int_distribution<intmax_t> uniform_printable_ASCII_dist(0x20, 0x7E);
            std::uniform_int_distribution<intmax_t> uniform_non_space_printable_ASCII_dist(0x21, 0x7E);
        }

        template<class T> constexpr bool is_sbc_type_v = std::is_same_v<T, char> || std::is_same_v<signed char> || std::is_same_v<unsigned char>;

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> printable_ASCII() {

        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> UCase_Latin() {

        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> LCase_Latin() {

        }

        template<class T> typename std::enable_if_t<is_sbc_type_v<T>> Dec_digit() {

        }

        
    }

    namespace Misc {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
