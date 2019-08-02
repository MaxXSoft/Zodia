#ifndef ZOIDA_UTIL_FILE_H_
#define ZOIDA_UTIL_FILE_H_

#include <string>

#include "util/os.h"

inline std::string GetBasePath(const std::string &file) {
  auto pos = file.rfind('/');
  if (pos == std::string::npos) return "";
  return file.substr(0, file.size() - pos);
}

inline std::string NormalizePath(const std::string &path) {
  if (!path.empty() && path.back() != '/') return path + '/';
  return path;
}

inline bool IsAbsPath(const std::string &path) {
#ifdef UTIL_OS_WINDOWS
  return path.size() >= 2 && path[1] == ':';
#else
  return !path.empty() && path[0] == '/';
#endif
}

#endif  // ZOIDA_UTIL_FILE_H_
