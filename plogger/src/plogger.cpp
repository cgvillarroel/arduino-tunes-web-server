#include "ansi.h"
#include "plogger.h"

Logger::Logger(HardwareSerial *serial) : ansi_(serial) { stream_ = serial; }

void Logger::setLogLevel(LogLevel level) { log_level_ = level; }

int Logger::logHeader(LogLevel level, const char *caller) {
  if (level < log_level_) {
    return 0;
  }

  int printed = 0;
  printed += stream_->print(F("["));

  switch (level) {
  case LogLevel::Debug:
    printed += ansi_.GreenFg();
    printed += stream_->print(F("DEBUG  "));
    break;
  case LogLevel::Info:
    printed += ansi_.BlueFg();
    printed += stream_->print(F("INFO   "));
    break;
  case LogLevel::Warning:
    printed += ansi_.YellowFg();
    printed += stream_->print(F("WARNING"));
    break;
  case LogLevel::Error:
    printed += ansi_.RedFg();
    printed += stream_->print(F("ERROR  "));
    break;
  case LogLevel::Fatal:
    printed += ansi_.RedBg();
    printed += ansi_.BlackFg();
    printed += stream_->print(F("FATAL  "));
  }

  printed += ansi_.Reset();
  printed += stream_->print(F("] ("));
  printed += ansi_.CyanFg();
  printed += stream_->print(caller);
  printed += ansi_.Reset();
  printed += stream_->print(F("): "));
  return printed;
}

int Logger::log(LogLevel level, const char *message, const char *caller) {
  if (level < log_level_) {
    return 0;
  }

  int printed = 0;
  printed += logHeader(level, caller);
  printed += stream_->println(message);
  return printed;
}

int Logger::debug(const char *message, const char *caller) {
  return log(LogLevel::Debug, message, caller);
}

int Logger::info(const char *message, const char *caller) {
  return log(LogLevel::Info, message, caller);
}

int Logger::warning(const char *message, const char *caller) {
  return log(LogLevel::Warning, message, caller);
}

int Logger::error(const char *message, const char *caller) {
  return log(LogLevel::Error, message, caller);
}

int Logger::fatal(const char *message, const char *caller) {
  return log(LogLevel::Fatal, message, caller);
}

int Logger::debug(const String &message, const char *caller) {
  return log(LogLevel::Debug, message.c_str(), caller);
}

int Logger::info(const String &message, const char *caller) {
  return log(LogLevel::Info, message.c_str(), caller);
}

int Logger::warning(const String &message, const char *caller) {
  return log(LogLevel::Warning, message.c_str(), caller);
}

int Logger::error(const String &message, const char *caller) {
  return log(LogLevel::Error, message.c_str(), caller);
}

int Logger::fatal(const String &message, const char *caller) {
  return log(LogLevel::Fatal, message.c_str(), caller);
}
