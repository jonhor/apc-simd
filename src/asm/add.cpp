#include <cstdint>
#include <iostream>

using vec256 = uint64_t[4];

int main() {
    vec256 res;
    vec256 a = {16402, 342, 45138, 3214};
    vec256 b = {87210, 23, 663971, 5101};

    asm(
        "vmovdqu %1, %%ymm0;"
        "vpaddq %2, %%ymm0, %%ymm1;"
        "vmovdqu %%ymm1, %0;"
        "vzeroupper;"
        : "=m"(res)
        : "m"(a), "m"(b)
        : "ymm0", "ymm1"
    );

    for (int i = 0; i < 4; ++i) {
        std::cout << res[i] << std::endl;
    }
}