// Copyright (c) 2016, GSI and The Polatory Authors.

#pragma once

#include <cmath>
#include <vector>

namespace polatory {
namespace common {

namespace detail {

static unsigned char bit_rev_table[256] = {
  0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240, 8, 136, 72, 200,
  40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248, 4, 132, 68, 196, 36, 164, 100, 228,
  20, 148, 84, 212, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220,
  60, 188, 124, 252, 2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
  10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250, 6, 134, 70, 198,
  38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238,
  30, 158, 94, 222, 62, 190, 126, 254, 1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209,
  49, 177, 113, 241, 9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
  5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245, 13, 141, 77, 205,
  45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253, 3, 131, 67, 195, 35, 163, 99, 227,
  19, 147, 83, 211, 51, 179, 115, 243, 11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219,
  59, 187, 123, 251, 7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
  15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255
};

template <size_t Size>
struct bit_rev {
private:
  static constexpr size_t NextSize = Size / 2;
  static constexpr size_t ShiftBits = NextSize * 8;

public:
  size_t operator()(size_t x) const {
    return (bit_rev<NextSize>()(x) << ShiftBits)
           | (bit_rev<NextSize>()(x >> ShiftBits));
  }
};

template <>
struct bit_rev<1> {
public:
  size_t operator()(size_t x) const {
    return bit_rev_table[static_cast<unsigned char>(x & 0xff)];
  }
};

inline
size_t bit_reverse(size_t x, int length) {
  return detail::bit_rev<sizeof(size_t)>()(x) >> (sizeof(size_t) * 8 - length);
}

}  // namespace detail

// Generates quasi-random sequence of integers within 0...n-1,
// based on binary van der Corput sequence preceded by 0.
inline
std::vector<size_t> quasi_random_sequence(size_t n) {
  std::vector<size_t> seq;

  if (n == 0) return seq;

  int n_digits = std::floor(std::log2(n)) + 1;
  size_t i_end = size_t(1) << n_digits;
  for (size_t i = 0; i < i_end; i++) {
    size_t i_rev = detail::bit_reverse(i, n_digits);
    if (i_rev >= n) continue;

    seq.push_back(i_rev);
    if (seq.size() == n) break;
  }

  return seq;
}

}  // namespace common
}  // namespace polatory
