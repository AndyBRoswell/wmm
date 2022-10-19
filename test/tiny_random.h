#ifndef WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
#define WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H

#include <chrono>
#include <cinttypes>
#include <iostream>
#include <random>
#include <stack>
#include <string>

namespace tiny_random {
    namespace {
        std::mt19937_64 random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<uintmax_t> max_uniform_uint_dist(0, UINTMAX_MAX);
        std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
    }

    namespace number {
        template<class T> constexpr T mod(const T& N, const T& D) { // N mod D. Note: mod is different from rem (%).
            const T r = N % D;
            if (D > 0) { return r >= 0 ? r : r + D; }
            else { return r <= 0 ? r : r + D; }
        }

        template<class T = uintmax_t> T integer() { // return a random integer between [INTMAX_MIN, INTMAX_MAX] or [0, UINTMAX_MAX]
            static_assert(std::is_integral_v<T>, "Only built-in integral types are allowed.");
            if constexpr (std::is_signed_v<T>) return max_uniform_int_dist(random_engine);
            else return max_uniform_uint_dist(random_engine);
        }

        template<class T = uintmax_t> T integer(const T a, const T b) { // return a random integer between [a, b], b - a <= max(T)
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
            dec = 1, hex, lhex, ucase, lcase, alpha, ualnum, lalnum, alnum, punct, printable,
        };

        template<class T> constexpr bool is_sbc_type_v = std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char> || std::is_same_v<T, char8_t>;

        template<class T = char> typename std::enable_if_t<is_sbc_type_v<T>, T> ASCII(const ASCII_char_type type = ASCII_char_type::printable) { // return an ASCII character
            using t = ASCII_char_type;
            switch (type) {
            case t::dec: return number::integer('0', '9');
            case t::hex: return hex[number::integer(0, 16 - 1)];
            case t::lhex: return lhex[number::integer(0, 16 - 1)];
            case t::ucase: return number::integer('A', 'Z');
            case t::lcase: return number::integer('a', 'z');
            case t::alpha: return latin[number::integer(0, 26 - 1)];
            case t::ualnum: return ualnum[number::integer(0, 10 + 26 - 1)];
            case t::lalnum: return lalnum[number::integer(0, 10 + 26 - 1)];
            case t::alnum: return alnum[number::integer(0, 10 + 26 + 26 - 1)];
            case t::punct: return punct[number::integer(0ull, sizeof(punct) - 1 - 1)]; // 0ull -> 0uz since C++23
            default: return number::integer(0x20, 0x7E);
            }
        }

        template<class T = char> typename std::basic_string<std::enable_if_t<is_sbc_type_v<T>, T>> ASCII_string(const size_t length, const ASCII_char_type type = ASCII_char_type::printable) { // return an ASCII string
            std::basic_string<T> s;
            for (size_t i = 0; i < length; ++i) { s.push_back(ASCII(type)); }
            return s;
        }

        template<class T = char> typename std::basic_string<std::enable_if_t<is_sbc_type_v<T>, T>> JSON() { // generate a JSON string which strictly comply the ECMA-262 3ed (Dec 1999)
            enum class state : char {
                object, array,                                                          // recursive structures
                string, number,                                                         // literals
                True, False, Null,                                                      // keywords
                value,                                                                  // value (specially processed)
                comma, lsquarebracket, rsquarebracket, lcurlybrace, rcurlybrace, colon, // punctuations
                space, htab, CR, LF,                                                    // whitespaces
            };
            enum class distribution { uniform, exponential }; // TODO: add "linear distribution"

            constexpr auto next_int = [](const auto& m, const auto& M) {return number::integer(m, M); };

            // parameters
            const size_t min_array_length = 1;
            const size_t max_array_length = 128;
            const size_t min_object_size = 1;
            const size_t max_object_size = 128;
            const size_t min_space_length = 1;
            const size_t max_space_length = 8;
            const size_t min_tab_count = 1;
            const size_t max_tab_count = 4;
            const distribution array_length_dist = distribution::exponential;
            const distribution object_size_dist = distribution::exponential;

            std::exponential_distribution<double> EXP(1);

            // workspace
            std::stack<state, std::vector<state>> S;
            std::basic_string<T> R;
            S.emplace(next_int(0, state::array + 1));
            while (S.empty() == false) {
                const state s = S.top();
                S.pop();
                switch (s) {
                case state::object: {
                    size_t n;
                    switch (object_size_dist) {
                    case distribution::uniform: n = next_int(min_object_size, max_object_size); break;
                    case distribution::exponential: n = std::min(std::max(max_object_size, std::min(min_object_size, static_cast<size_t>(next_int(min_object_size, max_object_size) * EXP(random_engine)))));
                    }
                } break;
                case state::array:
                case state::string:
                case state::number:
                case state::True:
                case state::False:
                case state::Null:
                case state::value:
                case state::comma:
                case state::lsquarebracket:
                case state::rsquarebracket:
                case state::lcurlybrace:
                case state::rcurlybrace:
                case state::colon:
                case state::space:
                case state::htab:
                case state::CR:
                case state::LF: break;
                }
            }
            return R;
        }
    }

    namespace Misc {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
