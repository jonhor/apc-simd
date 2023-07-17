#ifndef FIND_H_
#define FIND_H_

#include <cstdlib>
#include <string_view>

std::size_t find_naive(const std::string_view &haystack, char needle);
std::size_t find_avx2(const std::string_view &haystack, char needle);

#endif