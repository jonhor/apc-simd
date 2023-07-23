#include "find.h"

#include <immintrin.h>

#include <string>
#include <experimental/simd>
#include <iostream>

namespace stdx = std::experimental;


std::size_t find_naive(const std::string_view &haystack, char needle) {
    auto it = haystack.begin();

    size_t idx = 0;
    while (*it != '\0') {
        if (*it == needle) {
            return idx;
        }
        ++it, ++idx;
    }

    return std::string::npos;
}


std::size_t find_avx2_intrinsics(const std::string_view &haystack, char needle) {
    __m256i v, v_res;
    __m256i v_needle = _mm256_set1_epi8(needle);

    auto s = haystack.data();
    auto len = haystack.length();

    size_t idx = 0;
    int zf;
    while (len - idx >= 32) {
        v = _mm256_load_si256((const __m256i *)(s + idx));
        v_res = _mm256_cmpeq_epi8(v, v_needle);
        zf = _mm256_testz_si256(v_res, v_res);
        if (zf == 0) {
            break;
        }

        idx += 32;
    }

    for (int i = 0; i < len - idx; ++i)
    {
        if (s[idx + i] == needle)
        {
            return idx + i;
        }
    }

    return std::string::npos;
}


std::size_t find_std_simd(const std::string_view &haystack, char needle) {
    using char_vec = stdx::native_simd<char>;

    char_vec v_needle{needle};
    char_vec v;

    auto *s = haystack.data();
    auto len = haystack.length();

    size_t idx = 0;
    while (len - idx >= char_vec::size()) {
        v.copy_from(s + idx, stdx::vector_aligned);

        if (stdx::some_of(v == v_needle)) {
            break;
        }

        idx += char_vec::size();
    }

    for (int i = 0; i < len - idx; ++i) {
        if (s[idx + i] == needle) {
            return idx + i;
        }
    }

    return std::string::npos;
}


std::size_t find_avx2_asm(const std::string_view &haystack, char needle) {
    auto *s = haystack.data();
    auto len = haystack.length();

    bool match;

    asm(
        "vpbroadcastb %0, %%ymm0;"
        :
        : "m"(needle)
        : "ymm0"
    );

    std::size_t idx = 0;
    while (len - idx >= 32) {
        asm goto (
            "vmovdqa %0, %%ymm1;"
            "vpcmpeqb %%ymm0, %%ymm1, %%ymm2;"
            "vptest %%ymm2, %%ymm2;"
            "jnz %l1;"
            :
            : "m"(*(s + idx))
            : "ymm0", "ymm1", "ymm2"
            : out
        );

        idx += 32;
    }
    out:

    for (int i = 0; i < len - idx; ++i) {
        if (s[idx + i] == needle) {
            return idx + i;
        }
    }

    return std::string::npos;
}
