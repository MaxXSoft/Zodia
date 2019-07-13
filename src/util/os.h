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

#endif  // ZODIA_UTIL_OS_H_
