#ifndef ZODIA_UTIL_BASE64_H_
#define ZODIA_UTIL_BASE64_H_

#include <string>
#include <vector>
#include <cstdint>

std::string EncodeBase64(const std::vector<std::uint8_t> &buffer);
std::vector<std::uint8_t> DecodeBase64(const std::string &str);

#endif  // ZODIA_UTIL_BASE64_H_
