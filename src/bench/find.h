#ifndef FIND_H_
#define FIND_H_

#include <cstdlib>
#include <string_view>

std::size_t find_naive(const std::string_view &haystack, char needle);
std::size_t find_avx2_asm(const std::string_view &haystack, char needle);
std::size_t find_avx2_intrinsics(const std::string_view &haystack, char needle);
std::size_t find_std_simd(const std::string_view &haystack, char needle);

#endif