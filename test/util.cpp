#include "util.h"

#include <iostream>

void util::no_sync_with_stdio() noexcept {
    if (std::ios_base::sync_with_stdio() == true) { std::ios_base::sync_with_stdio(false); }
    if (std::cin.tie() != nullptr) { std::cin.tie(nullptr); }
    if (std::cout.tie() != nullptr) { std::cout.tie(nullptr); }
}
