/*

  base64.cpp and base64.h

  base64 encoding and decoding with C++.

  Version: 1.01.00

  Copyright (C) 2004-2017 René Nyffenegger

  This source code is provided 'as-is', without any express or implied
  warranty. In no event will the author be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this source code must not be misrepresented; you must
  not claim that you wrote the original source code. If you use this source
  code in a product, an acknowledgment in the product documentation would
  be appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not
  be misrepresented as being the original source code.

  3. This notice may not be removed or altered from any source
  distribution.

  René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

// Modified by MaxXing

#include "util/base64.h"

#include <cctype>

namespace {

static std::string table =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

inline bool IsBase64(std::uint8_t c) {
  return (std::isalnum(c) || (c == '+') || (c == '/'));
}

}  // namespace

std::string EncodeBase64(const std::vector<std::uint8_t>& buffer) {
  std::string ret;
  int i = 0, j = 0;
  std::uint8_t chars_3[3], chars_4[4];
  // convert to base64 characters
  for (const auto& c : buffer) {
    chars_3[i++] = c;
    if (i == 3) {
      chars_4[0] = (chars_3[0] & 0xfc) >> 2;
      chars_4[1] = ((chars_3[0] & 0x03) << 4) + ((chars_3[1] & 0xf0) >> 4);
      chars_4[2] = ((chars_3[1] & 0x0f) << 2) + ((chars_3[2] & 0xc0) >> 6);
      chars_4[3] = chars_3[2] & 0x3f;
      for (i = 0; (i < 4); i++) ret += table[chars_4[i]];
      i = 0;
    }
  }
  // handle padding
  if (i) {
    for (j = i; j < 3; j++) chars_3[j] = '\0';
    chars_4[0] = (chars_3[0] & 0xfc) >> 2;
    chars_4[1] = ((chars_3[0] & 0x03) << 4) + ((chars_3[1] & 0xf0) >> 4);
    chars_4[2] = ((chars_3[1] & 0x0f) << 2) + ((chars_3[2] & 0xc0) >> 6);
    for (j = 0; (j < i + 1); j++) ret += table[chars_4[j]];
    while ((i++ < 3)) ret += '=';
  }
  return ret;
}

std::vector<std::uint8_t> DecodeBase64(const std::string& str) {
  int in_len = str.size();
  int i = 0, j = 0, in = 0;
  std::uint8_t chars_4[4], chars_3[3];
  std::vector<std::uint8_t> ret;
  // convert base64 characters to bytes
  while (in_len-- && (str[in] != '=') && IsBase64(str[in])) {
    chars_4[i++] = str[in];
    in++;
    if (i == 4) {
      for (i = 0; i < 4; i++) chars_4[i] = table.find(chars_4[i]);
      chars_3[0] = (chars_4[0] << 2) + ((chars_4[1] & 0x30) >> 4);
      chars_3[1] = ((chars_4[1] & 0xf) << 4) + ((chars_4[2] & 0x3c) >> 2);
      chars_3[2] = ((chars_4[2] & 0x3) << 6) + chars_4[3];
      for (i = 0; (i < 3); i++) ret.push_back(chars_3[i]);
      i = 0;
    }
  }
  // handle padding
  if (i) {
    for (j = 0; j < i; j++) chars_4[j] = table.find(chars_4[j]);
    chars_3[0] = (chars_4[0] << 2) + ((chars_4[1] & 0x30) >> 4);
    chars_3[1] = ((chars_4[1] & 0xf) << 4) + ((chars_4[2] & 0x3c) >> 2);
    for (j = 0; (j < i - 1); j++) ret.push_back(chars_3[j]);
  }
  return ret;
}
