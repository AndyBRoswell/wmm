#ifndef WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
#define WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H

#include <chrono>
#include <cinttypes>
#include <iostream>
#include <map>
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

        template<class T> constexpr bool is_sbc_type_v = std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char> || std::is_same_v<T, char8_t>;

        template<class T> using enable_if_sbc = std::enable_if_t<is_sbc_type_v<T>, T>;  // sbc -> single-byte character
        template<class T> using enable_if_sbs = std::basic_string<enable_if_sbc<T>>;    // sbs -> single-byte string

        enum class ASCII_char_type {
            dec = 1, hex, lhex, ucase, lcase, alpha, ualnum, lalnum, alnum, punct, printable,
        };

        template<class T = char> typename enable_if_sbc<T> ASCII(const ASCII_char_type type = ASCII_char_type::printable) { // return an ASCII character
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

        template<class T = char> typename enable_if_sbs<T> ASCII_string(const size_t length, const ASCII_char_type type = ASCII_char_type::printable) { // return an ASCII string
            std::basic_string<T> s;
            for (size_t i = 0; i < length; ++i) { s.push_back(ASCII(type)); }
            return s;
        }

        template<class T = char> typename enable_if_sbs<T> JSON() { // return a random JSON string which strictly comply the ECMA-262 3ed (Dec 1999) but with random whitespaces
            enum class state : char {
                value,                                                                      // value
                object, array,                                                              // recursive structures
                string, number,                                                             // literals
                True, False, Null,                                                          // keywords
                comma, left_square, right_square, left_curly, right_curly, colon, quote,    // punctuations
                whitespace,                                                                 // whitespace
                space, horizontal_tab, CR, LF,                                              // whitespace
            };
            enum class distribution { uniform, exponential }; // TODO: add "linear distribution"
            static const std::map<state, std::basic_string<T>> direct_input = {
                { state::True, "True" }, { state::False, "False" }, { state::Null, "null" },
                { state::comma, "," }, { state::left_square, "[" }, { state::right_square, "]" }, { state::left_curly, "{" }, { state::right_curly, "}" }, { state::colon, ":" },
                { state::space, " " }, { state::horizontal_tab, "\t" }, { state::CR, "\r" }, { state::LF, "\n" },
            };
            static std::exponential_distribution<double> EXP(1);

            constexpr auto next_int = [](const auto& m, const auto& M, const distribution D = distribution::uniform) { 
                switch (D) {
                //case distribution::uniform: return number::integer(m, M);
                //case distribution::exponential: return std::min(std::max(M, std::min(m, static_cast<decltype(M)>(number::integer(m, M) * EXP(random_engine)))));
                }
                return m;
            };

            // parameters
            const size_t min_arr_size = 1, max_arr_size = 256;
            const size_t min_obj_size = 1, max_obj_size = 256;
            const size_t min_str_len = 1, max_str_len = 256;
            const double p_escape = 0.05;
            const size_t min_single_ws_len = 0, max_single_ws_len = 8;
            const size_t min_ws_count = 0, max_ws_count = 8;
            const distribution arr_len_dist = distribution::exponential;
            const distribution obj_size_dist = distribution::exponential;
            const distribution str_len_dist = distribution::exponential;
            const distribution single_ws_len_dist = distribution::exponential;
            const distribution ws_count_dist = distribution::exponential;

            // workspace
            std::stack<state, std::vector<state>> S;
            std::basic_string<T> R;
            S.emplace(next_int(state::object, state::array));
            while (S.empty() == false) { // non-recursive
                const state s = S.top();
                S.pop();
                switch (s) {
                case state::object: {
                    const size_t n = next_int(min_obj_size, max_obj_size, obj_size_dist);
                    S.emplace(state::right_curly); S.emplace(state::whitespace);
                    S.emplace(state::value); S.emplace(state::whitespace);
                    S.emplace(state::colon); S.emplace(state::whitespace);
                    S.emplace(state::string); S.emplace(state::whitespace);
                    for (size_t i = 1; i < n; ++i) {
                        S.emplace(state::comma); S.emplace(state::whitespace);
                        S.emplace(state::value); S.emplace(state::whitespace);
                        S.emplace(state::colon); S.emplace(state::whitespace);
                        S.emplace(state::string); S.emplace(state::whitespace);
                    }
                    S.emplace(state::left_curly);
                } break;
                case state::array: {
                    const size_t n = next_int(min_arr_size, max_arr_size, arr_len_dist);
                    S.emplace(state::right_square); S.emplace(state::whitespace);
                    for (size_t i = 1; i < n; ++i) {
                        S.emplace(state::comma); S.emplace(state::whitespace);
                        S.emplace(state::value); S.emplace(state::whitespace);
                    }
                    S.emplace(state::left_square);
                } break;
                case state::value: {
                    S.emplace(state::whitespace);
                    S.emplace(next_int(state::object, state::Null));
                    S.emplace(state::whitespace);
                } break;
                case state::whitespace: {
                    const size_t n = next_int(min_ws_count, max_ws_count, ws_count_dist);
                    for (size_t i = 0; i < n; ++i) { S.emplace(next_int(state::space, state::LF)); }
                } break;
                case state::string: {
                    static std::uniform_real_distribution<double> U(0, 1);
                    static constexpr char esc[] = R"("\/bfnrtu)";
                    R.push_back('\"');
                    const size_t n = next_int(min_str_len, max_str_len, str_len_dist);
                    for (size_t i = 0; i < n; ++i) {
                        if (U(random_engine) <= p_escape) { // generate an escape character
                            const char e[] = { '\\', esc[next_int(0ui64, sizeof(esc) - 1 - 1)], '\0' };
                            R.append(e);
                            if (e[1] == 'u') {
                                const uint16_t h = next_int(0ui16, UINT16_MAX);
                                for (uint16_t i = 0, d = 1; i < 4; ++i, d *= 16) { R.push_back(h / d % 16 + '0'); } // convert to hex
                            }
                        }
                        else { R.push_back(ASCII()); } // generate normal character;
                    }
                    R.push_back('\"');
                } break;
                case state::number: {
                    const bool negative = next_int(0, 1);
                    if (negative) R.push_back('-');
                    switch (next_int(0, 1)) {
                    default: R.append(std::to_string(next_int(0i64, INT64_MAX, distribution::exponential))); break; // int
                    case 1: { // float
                        switch (next_int(0, 1)) {
                        default: { // no scientific notation
                            static std::uniform_real_distribution<double> U(0, 999999999999999);
                            R.append(std::to_string(U(random_engine)* EXP(random_engine)));
                        } break;
                        case 1: { // maybe has scientific notation
                            static std::uniform_real_distribution<double> U(0, DBL_MAX);
                            R.append(std::to_string(U(random_engine)* EXP(random_engine)));
                        } break;
                        }
                    } break;
                    }
                } break;
                case state::space: case state::horizontal_tab: case state::CR: case state::LF: {
                    const size_t n = next_int(min_single_ws_len, max_single_ws_len, single_ws_len_dist);
                    for (size_t i = 0; i < n; ++i) { R.append(direct_input.at(s)); }
                } break;
                default: { R.append(direct_input.at(s)); } break;
                }
            }
            return R;
        }
    }

    namespace Misc {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
