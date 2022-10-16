#ifndef WRITING_MATERIALS_MANAGER_TESTS_RANDOM_H
#define WRITING_MATERIALS_MANAGER_TESTS_RANDOM_H

#include <chrono>
#include <random>

namespace TinyRandom {
    std::mt19937_64 default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<uintmax_t> max_uniform_uint_dist(0, UINTMAX_MAX);
    std::uniform_int_distribution<intmax_t> max_uniform_int_dist(INTMAX_MIN, INTMAX_MAX);
    
    namespace Number {
        // return a random integer between [0, UINTMAX_MAX]
        template<class T> RanUInt() {
            static_assert(std::is_unsigned_v<T>, "Only unsigned types are allowed.");
            return max_uniform_uint_dist(default_random_engine);
        }
        
        // return a random integer between [a, b]
        template<class T> RanUInt(const T a, const T b) {
            
        }

        template<class T> RanInt() {
            static_assert(std::signed_v<T>, "Only signed types are allowed.");
            return max_uniform_int_dist(default_random_engine);
        }

        // return a random integer between [a, b]
        template<class T> RanInt(const T a, const T b) {

        }
    }

    namespace String {

    }
}

#endif //WRITING_MATERIALS_MANAGER_TESTS_RANDOM_H
