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
        template<class T> constexpr T mod(const T& N, const T& D) noexcept { // N mod D. Note: mod is different from rem (%).
            const T r = N % D;
            if (D > 0) { return r >= 0 ? r : r + D; }
            else { return r <= 0 ? r : r + D; }
        }

        template<class T = uintmax_t > T integer() noexcept { // return a random integer between [INTMAX_MIN, INTMAX_MAX] or [0, UINTMAX_MAX]
            static_assert(std::is_integral_v<T>, "Only built-in integral types are allowed.");
            if constexpr (std::is_signed_v<T>) return max_uniform_int_dist(random_engine);
            else return max_uniform_uint_dist(random_engine);
        }

        /* Return a random integer between[a, b], a <= b.
        If a, b are of signed types, then b - a <= INTMAX_MAX / 2 - INTMAX_MIN / 2 = pow(2, 63) - 1; 
        If a, b are of unsigned types, then b - a <= UINTMAX_MAX - 0 = pow(2, 64) - 1. */
        template<class T = uintmax_t> T integer(const T a, const T b) noexcept {
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
            constexpr char punct[] = "!\"#$%&'()*+,-./"";:<=>?@""[\\]^_`{|}~";
            constexpr char JSON_non_esc[] = "!#$%&'()*+,-./;0123456789;:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~";
            constexpr char JSON_esc[] = R"("\/fnrt)"; // no \b \u
        }

        template<class T> constexpr bool is_sbc_type_v = std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char> || std::is_same_v<T, char8_t>;

        template<class T> using enable_if_sbc = std::enable_if_t<is_sbc_type_v<T>, T>;  // sbc -> single-byte character
        template<class T> using enable_if_sbs = std::basic_string<enable_if_sbc<T>>;    // sbs -> single-byte string

        enum class ASCII_char_type {
            dec = 1, hex, lhex, ucase, lcase, alpha, ualnum, lalnum, alnum, punct, printable,
        };

        template<class T = char> typename enable_if_sbc<T> ASCII(const ASCII_char_type type = ASCII_char_type::printable) noexcept { // return an ASCII character
            using t = ASCII_char_type;
            switch (type) {
            case t::dec: return number::integer('0', '9');
            case t::hex: return hex[number::integer(0, 16 - 1)];
            case t::lhex: return lhex[number::integer(0, 16 - 1)];
            case t::ucase: return number::integer('A', 'Z');
            case t::lcase: return number::integer('a', 'z');
            case t::alpha: return latin[number::integer(0, 26 + 26 - 1)];
            case t::ualnum: return ualnum[number::integer(0, 10 + 26 - 1)];
            case t::lalnum: return lalnum[number::integer(0, 10 + 26 - 1)];
            case t::alnum: return alnum[number::integer(0, 10 + 26 + 26 - 1)];
            case t::punct: return punct[number::integer(0ull, sizeof(punct) - 1 - 1)]; // 0ull -> 0uz since C++23
            default: return number::integer(0x20, 0x7E);
            }
        }

        template<class T = char> typename enable_if_sbs<T> ASCII_string(const size_t length, const ASCII_char_type type = ASCII_char_type::printable) noexcept { // return an ASCII string
            std::basic_string<T> s;
            for (size_t i = 0; i < length; ++i) { s.push_back(ASCII(type)); }
            return s;
        }

        template<class T = char> typename enable_if_sbs<T> JSON() noexcept { // return a random JSON string which strictly comply the RFC 4627 (Jul 2006) but with random whitespaces
            enum class state : char {
                value,                                                  // value
                object, array,                                          // recursive structures
                string, number,                                         // literals
                True, False, Null,                                      // keywords
                comma, lsquare, rsquare, lcurly, rcurly, colon, quote,  // punctuations
                whitespace,                                             // whitespace
                space, htab, CR, LF,                                    // whitespace
            };
            enum class distribution { uniform, exponential }; // TODO: add "interval linear distribution"
            static const std::map<state, std::basic_string<T>> direct_input = {
                { state::True, "true" }, { state::False, "false" }, { state::Null, "null" },
                { state::comma, "," }, { state::lsquare, "[" }, { state::rsquare, "]" }, { state::lcurly, "{" }, { state::rcurly, "}" }, { state::colon, ":" },
                { state::space, " " }, { state::htab, "\t" }, { state::CR, "\r" }, { state::LF, "\n" },
            };
            static std::exponential_distribution<double> EXP(2);

            constexpr auto next_int = []<class T>(const T m, const T M, const distribution D = distribution::uniform) noexcept {
                switch (D) {
                default: return number::integer(m, M);
                case distribution::exponential:
                    if constexpr (std::is_signed_v<T>) { return std::min(M, std::max(m, static_cast<T>((number::integer(0, 1) == 0 ? 1 : -1) * number::integer(m, M) * EXP(random_engine)))); }
                    else { return std::min(M, std::max(m, static_cast<T>(number::integer(m, M) * EXP(random_engine)))); }
                }
            };
            constexpr auto next_enum = []<class T>(const T m, const T M) noexcept {
                return static_cast<T>(number::integer(static_cast<std::underlying_type_t<T>>(m), static_cast<std::underlying_type_t<T>>(M)));
            };

            // parameters
            const size_t min_arr_size = 0, max_arr_size = 128;
            const size_t min_obj_size = 0, max_obj_size = 128;
            const size_t min_str_len = 0, max_str_len = 64; 
            const double p_escape = 0.01;
            const size_t min_single_ws_len = 0, max_single_ws_len = 8;
            const size_t min_ws_count = 0, max_ws_count = 2;
            const size_t fp_precision = DBL_DECIMAL_DIG - 2;
            const size_t max_recursive_depth = 6;
            const distribution arr_len_dist = distribution::exponential;
            const distribution obj_size_dist = distribution::exponential;
            const distribution str_len_dist = distribution::exponential;
            const distribution single_ws_len_dist = distribution::exponential;
            const distribution ws_count_dist = distribution::exponential;
            { // workspace
                using enum state;

                std::stack<state, std::vector<state>> S;
                std::basic_string<T> ret;
                size_t recursive_depth = 0;
                S.emplace(next_enum(object, array));
                while (S.empty() == false) { // non-recursive
                    const state s = S.top();
                    S.pop();
                    switch (s) {
                    case object: {
                        const size_t n = next_int(min_obj_size, max_obj_size, obj_size_dist);
                        S.emplace(rcurly); S.emplace(whitespace);
                        S.emplace(value); S.emplace(whitespace);
                        S.emplace(colon); S.emplace(whitespace);
                        S.emplace(string); S.emplace(whitespace);
                        for (size_t i = 1; i < n; ++i) {
                            S.emplace(comma); S.emplace(whitespace);
                            S.emplace(value); S.emplace(whitespace);
                            S.emplace(colon); S.emplace(whitespace);
                            S.emplace(string); S.emplace(whitespace);
                        }
                        S.emplace(lcurly);
                    } break;
                    case array: {
                        const size_t n = next_int(min_arr_size, max_arr_size, arr_len_dist);
                        S.emplace(rsquare); S.emplace(whitespace);
                        S.emplace(value); S.emplace(whitespace);
                        for (size_t i = 1; i < n; ++i) {
                            S.emplace(comma); S.emplace(whitespace);
                            S.emplace(value); S.emplace(whitespace);
                        }
                        S.emplace(lsquare);
                    } break;
                    case value: {
                        S.emplace(whitespace);
                        if (recursive_depth < max_recursive_depth) { S.emplace(next_enum(object, Null)); }
                        else { S.emplace(next_enum(string, Null)); }
                        S.emplace(whitespace);
                    } break;
                    case whitespace: {
                        const size_t n = next_int(min_ws_count, max_ws_count, ws_count_dist);
                        for (size_t i = 0; i < n; ++i) { S.emplace(next_enum(space, LF)); }
                    } break;
                    case string: {
                        static std::uniform_real_distribution<double> U(0, 1);
                        ret.push_back('\"');
                        const size_t n = next_int(min_str_len, max_str_len, str_len_dist);
                        for (size_t i = 0; i < n; ++i) {
                            if (U(random_engine) <= p_escape) { // generate an escape character
                                const char e[] = { '\\', JSON_esc[next_int(0ui64, sizeof(JSON_esc) - 1 - 1)], '\0' };
                                ret.append(e);
                                //if (e[1] == 'u') {
                                //    const int h = next_int(0x00A0ui32, static_cast<uint32_t>(UINT16_MAX));
                                //    for (int i = 3, d = UINT16_MAX; i >= 0; --i, d /= 16) { ret.push_back(hex[h / d % 16]); } // convert to hex
                                //}
                            }
                            else { ret.push_back(JSON_non_esc[next_int(0ui64, sizeof(JSON_non_esc) - 1 - 1)]); } // generate normal character;
                        }
                        ret.push_back('\"');
                    } break;
                    case number: {
                        const bool negative = next_int(0, 1);
                        if (negative) ret.push_back('-');
                        switch (next_int(0, 1)) {
                        default: ret.append(std::to_string(next_int(0i64, INT64_MAX, distribution::exponential))); break; // int
                        case 1: { // float
                            switch (next_int(0, 1)) {
                            default: { // no scientific notation
                                static std::uniform_real_distribution<double> U(0, 10);
                                const auto out_str = std::format("{:.{}f}", U(random_engine) * EXP(random_engine), fp_precision);
                                ret.append(out_str);
                            } break;
                            case 1: { // scientific notation
                                static std::uniform_real_distribution<double> U(1, 10);
                                const double coef = U(random_engine);
                                const int exp = next_int(-DBL_MAX_10_EXP, DBL_MAX_10_EXP, distribution::exponential);
                                if (const double r = coef * pow(10, exp); isinf(r)) { ret.append(std::format("{:.{}f}", DBL_MAX, fp_precision)); }
                                else {
                                    const auto coef_str = std::format("{:.{}f}", coef, fp_precision);
                                    ret.append(coef_str + (next_int(0, 1) == 0 ? "E" : "e"));
                                    if (exp < 0) { ret.append(std::to_string(exp)); }
                                    else { ret.append((next_int(0, 1) == 0 ? "" : "+") + std::to_string(exp)); }
                                }
                            } break;
                            }
                        } break;
                        }
                    } break;
                    case space: case htab: case CR: case LF: {
                        const size_t n = next_int(min_single_ws_len, max_single_ws_len, single_ws_len_dist);
                        for (size_t i = 0; i < n; ++i) { ret.append(direct_input.at(s)); }
                    } break;
                    case lcurly: case lsquare: {
                        ++recursive_depth;
                        ret.append(direct_input.at(s));
                    } break;
                    case rcurly: case rsquare: {
                        --recursive_depth;
                        ret.append(direct_input.at(s));
                    } break;
                    default: { ret.append(direct_input.at(s)); } break;
                    }
                }
                return ret;
            }
        }
    }

    namespace Misc {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_TINY_RANDOM_H
