#ifndef PLOGGER_H
#define PLOGGER_H

#include <Arduino.h>

#include "ansi.h"

enum class LogLevel {
  Debug,
  Info,
  Warn,
  Error,
  Fatal,
};

/// Logs information to the terminal with formatting
class Logger {
  LogLevel log_level_ = LogLevel::Info;
  bool prev_ended = true;
  Stream *stream_;
  Ansi ansi_;

  int logHeader(LogLevel, const char *);

public:
  Logger(HardwareSerial *);
  void setLogLevel(LogLevel);

  int log(LogLevel, const char *, const char *caller = __builtin_FUNCTION());
  int logLine(LogLevel, const char *,
              const char *caller = __builtin_FUNCTION());

  int debug(const char *, const char *caller = __builtin_FUNCTION());
  int info(const char *, const char *caller = __builtin_FUNCTION());
  int warn(const char *, const char *caller = __builtin_FUNCTION());
  int error(const char *, const char *caller = __builtin_FUNCTION());
  int fatal(const char *, const char *caller = __builtin_FUNCTION());

  int debug(const String &, const char *caller = __builtin_FUNCTION());
  int info(const String &, const char *caller = __builtin_FUNCTION());
  int warn(const String &, const char *caller = __builtin_FUNCTION());
  int error(const String &, const char *caller = __builtin_FUNCTION());
  int fatal(const String &, const char *caller = __builtin_FUNCTION());

  int debugLine(const char *, const char *caller = __builtin_FUNCTION());
  int infoLine(const char *, const char *caller = __builtin_FUNCTION());
  int warnLine(const char *, const char *caller = __builtin_FUNCTION());
  int errorLine(const char *, const char *caller = __builtin_FUNCTION());
  int fatalLine(const char *, const char *caller = __builtin_FUNCTION());

  int debugLine(const String &, const char *caller = __builtin_FUNCTION());
  int infoLine(const String &, const char *caller = __builtin_FUNCTION());
  int warnLine(const String &, const char *caller = __builtin_FUNCTION());
  int errorLine(const String &, const char *caller = __builtin_FUNCTION());
  int fatalLine(const String &, const char *caller = __builtin_FUNCTION());
};

#endif // PLOGGER_H
