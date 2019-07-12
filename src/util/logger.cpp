#include <util/logger.h>

#include <cassert>
#include <ctime>
#include <iostream>

#include <SDL2/SDL.h>

bool Logger::enabled_ = true;
std::ostream *Logger::os_ = &std::cerr;

void Logger::LogHeader(const std::string &log_type) {
  char buffer[100];
  auto format = "%Y-%m-%d %T";
  auto now = std::time(nullptr);
  if (std::strftime(buffer, 100, format, std::localtime(&now))) {
    *os_ << buffer << ' ';
  }
  *os_ << '[' << log_type << "] ";
}

void Logger::LogError(const std::string &message) {
  if (!enabled_) return;
  auto error_msg = SDL_GetError();
  LogHeader("error");
  *os_ << message;
  if (error_msg && error_msg[0] != '\0') {
    *os_ << ", SDL message: " << error_msg;
  }
  *os_ << std::endl;
}

void Logger::LogInfo(const std::string &message) {
  if (!enabled_) return;
  LogHeader("info");
  *os_ << message << std::endl;
}

void Logger::LogDebug(const std::string &message) {
#if DEBUG
  if (!enabled_) return;
  LogHeader("debug");
  *os_ << message << std::endl;
#endif
}
