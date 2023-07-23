#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "third_party/catch.hpp"

#include "find.h"


TEST_CASE("find character in string")
{
    constexpr std::size_t slen = 1000 * 1000;
    constexpr std::size_t align = 32;

    char *bytes = (char *) std::aligned_alloc(align, slen);
    if (bytes == nullptr) {
        std::cerr << "could not allocate " << slen << " bytes" << std::endl;
        exit(1);
    }

    std::memset(bytes, 'a', slen);
    bytes[slen - 1] = '\0';
    bytes[999000] = 'b';

    std::string_view haystack(bytes, slen);
    
    char needle = 'b';

    CHECK(find_naive(haystack, needle) == 999000);
    CHECK(find_avx2_asm(haystack, needle) == 999000);
    CHECK(find_avx2_intrinsics(haystack, needle) == 999000);
    CHECK(find_std_simd(haystack, needle) == 999000);

    BENCHMARK("find.h find_naive")
    {
        return find_naive(haystack, needle);
    };

    BENCHMARK("find.h find_avx2_asm")
    {
        return find_avx2_asm(haystack, needle);
    };

    BENCHMARK("find.h find_avx2_intrinsics")
    {
        return find_avx2_intrinsics(haystack, needle);
    };

    BENCHMARK("find.h find_std_simd")
    {
        return find_std_simd(haystack, needle);
    };

    BENCHMARK("std::string_view::find")
    {
        return haystack.find(needle);
    };

    BENCHMARK("std::find") {
        return std::find(haystack.begin(), haystack.end(), needle);
    };

    BENCHMARK("std::strchr") {
        return std::strchr(bytes, needle);
    };

    std::free(bytes);
}