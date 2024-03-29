#ifndef ZODIA_UTIL_OS_H_
#define ZODIA_UTIL_OS_H_

// some macros about current operating system

#if defined(_WIN64) || defined(__MINGW64__) || \
    (defined(__CYGWIN__) && defined(__x86_64__))
#define UTIL_OS_64_WIN
#elif defined(__x86_64__)
#define UTIL_OS_64_GCC
#endif

#if !defined(UTIL_OS_64) && !defined(UTIL_OS_32)
#if defined(UTIL_OS_64_GCC) || defined(UTIL_OS_64_WIN)
#define UTIL_OS_64
#else
#define UTIL_OS_32
#endif
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)
#define UTIL_OS_WINDOWS
#ifdef _MSC_VER
#define UTIL_OS_WIN_VS
#endif
#endif

#ifdef __APPLE__
#define UTIL_OS_MACOS
#endif

#endif  // ZODIA_UTIL_OS_H_
