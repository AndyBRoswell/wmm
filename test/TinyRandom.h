#ifndef WRITING_MATERIALS_MANAGER_TESTS_RANDOM_H
#define WRITING_MATERIALS_MANAGER_TESTS_RANDOM_H

#include <chrono>
#include <random>

namespace TinyRandom {
    std::mt19937_64 default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<uintmax_t> max_uniform_uint_dist(0, UINTMAX_MAX);
    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
    
    namespace Number {
        template<class T> T RanInt() {
            static_assert(std::is_arithmetic_v<T>, "Only built-in arithmetic types are allowed.");
            if constexpr (std::is_signed_v<T>) return max_uniform_int_dist(default_random_engine);
            else return max_uniform_uint_dist(default_random_engine);
        }

        // return a random integer between [a, b], b - a <= max(T)
        template<class T> T RanInt(const T a, const T b) {
            static_assert(std::is_arithmetic_v<T>, "Only built-in arithmetic types are allowed.");
            const T L = b - a;
            if constexpr (std::is_signed_v<T>) {
                const T x = max_uniform_int_dist(default_random_engine);
                return a + (x % L - INTMAX_MIN % L) % L;
            }
            else {
                const T x = max_uniform_uint_dist(default_random_engine);
                return a + x % L;
            }
        }
    }

    namespace String {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_RANDOM_H
