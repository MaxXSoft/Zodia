#ifndef ZODIA_UTIL_LOGGER_H_
#define ZODIA_UTIL_LOGGER_H_

#include <ostream>
#include <string>
#include <stdexcept>

// macro for logging errors
#define LOG_ERROR(msg)                \
    do {                              \
      Logger::LogError(__func__);     \
      throw std::runtime_error(msg);  \
    } while (0)

class Logger {
 public:
  static void SetEnabled(bool enabled) { enabled_ = enabled; }
  static void SetOutputStream(std::ostream &os) { os_ = &os; }

  static void LogError(const std::string &message);
  static void LogInfo(const std::string &message);
  static void LogDebug(const std::string &message);

 private:
  Logger() {}

  static void LogHeader(const std::string &log_type);

  static bool enabled_;
  static std::ostream *os_;
};

#endif  //ZODIASUTILNE_LOGGER_H_
