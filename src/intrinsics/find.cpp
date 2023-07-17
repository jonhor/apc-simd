#include "find.h"

#include <immintrin.h>

#include <string>


std::size_t find_naive(const std::string_view &haystack, char needle) {
    auto it = haystack.begin();

    size_t idx = 0;
    while (*it != '\0') {
        if (*it == needle)
        {
            return idx;
        }
        ++it, ++idx;
    }

    return std::string::npos;
}


std::size_t find_avx2(const std::string_view &haystack, char needle)
{
    __m256i v, v_res;
    __m256i v_needle = _mm256_set1_epi8(needle);

    auto s = haystack.data();
    auto len = haystack.length();

    size_t idx = 0;
    int zf;
    while (len - idx >= 32)
    {
        v = _mm256_load_si256((const __m256i *)(s + idx));
        v_res = _mm256_cmpeq_epi8(v, v_needle);
        zf = _mm256_testz_si256(v_res, v_res);
        if (zf == 0)
        {
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