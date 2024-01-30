#ifndef PLOGGER_H
#define PLOGGER_H

#include <Arduino.h>

#include "ansi.h"

enum class LogLevel {
  Debug,
  Info,
  Warning,
  Error,
  Fatal,
};

/// Logs information to the terminal with formatting
class Logger {
  LogLevel log_level_ = LogLevel::Info;
  Stream *stream_;
  Ansi ansi_;

public:
  Logger(HardwareSerial *);
  void setLogLevel(LogLevel);

  int log(LogLevel, const char *, const char *caller = __builtin_FUNCTION());
  int logHeader(LogLevel, const char *caller = __builtin_FUNCTION());
  int debug(const char *, const char *caller = __builtin_FUNCTION());
  int info(const char *, const char *caller = __builtin_FUNCTION());
  int warning(const char *, const char *caller = __builtin_FUNCTION());
  int error(const char *, const char *caller = __builtin_FUNCTION());
  int fatal(const char *, const char *caller = __builtin_FUNCTION());

  int log(LogLevel, const String &, const char *caller = __builtin_FUNCTION());
  int debug(const String &, const char *caller = __builtin_FUNCTION());
  int info(const String &, const char *caller = __builtin_FUNCTION());
  int warning(const String &, const char *caller = __builtin_FUNCTION());
  int error(const String &, const char *caller = __builtin_FUNCTION());
  int fatal(const String &, const char *caller = __builtin_FUNCTION());
};

#endif // PLOGGER_H
