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
    printed += stream_->print(F("DEBUG"));
    break;
  case LogLevel::Info:
    printed += ansi_.BlueFg();
    printed += stream_->print(F("INFO "));
    break;
  case LogLevel::Warn:
    printed += ansi_.YellowFg();
    printed += stream_->print(F("WARN "));
    break;
  case LogLevel::Error:
    printed += ansi_.RedFg();
    printed += stream_->print(F("ERROR"));
    break;
  case LogLevel::Fatal:
    printed += ansi_.RedBg();
    printed += ansi_.BlackFg();
    printed += stream_->print(F("FATAL"));
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
  if (prev_ended) {
    printed += logHeader(level, caller);
  }
  printed += stream_->print(message);

  prev_ended = false;
  return printed;
}

int Logger::logLine(LogLevel level, const char *message, const char *caller) {
  if (level < log_level_) {
    return 0;
  }

  int printed = 0;

  printed += log(level, message, caller);
  printed += stream_->println();

  prev_ended = true;
  return printed;
}

int Logger::debug(const char *message, const char *caller) {
  return log(LogLevel::Debug, message, caller);
}

int Logger::info(const char *message, const char *caller) {
  return log(LogLevel::Info, message, caller);
}

int Logger::warn(const char *message, const char *caller) {
  return log(LogLevel::Warn, message, caller);
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

int Logger::warn(const String &message, const char *caller) {
  return log(LogLevel::Warn, message.c_str(), caller);
}

int Logger::error(const String &message, const char *caller) {
  return log(LogLevel::Error, message.c_str(), caller);
}

int Logger::fatal(const String &message, const char *caller) {
  return log(LogLevel::Fatal, message.c_str(), caller);
}

int Logger::debugLine(const char *message, const char *caller) {
  return logLine(LogLevel::Debug, message, caller);
}

int Logger::infoLine(const char *message, const char *caller) {
  return logLine(LogLevel::Info, message, caller);
}

int Logger::warnLine(const char *message, const char *caller) {
  return logLine(LogLevel::Warn, message, caller);
}

int Logger::errorLine(const char *message, const char *caller) {
  return logLine(LogLevel::Error, message, caller);
}

int Logger::fatalLine(const char *message, const char *caller) {
  return logLine(LogLevel::Fatal, message, caller);
}

int Logger::debugLine(const String &message, const char *caller) {
  return logLine(LogLevel::Debug, message.c_str(), caller);
}

int Logger::infoLine(const String &message, const char *caller) {
  return logLine(LogLevel::Info, message.c_str(), caller);
}

int Logger::warnLine(const String &message, const char *caller) {
  return logLine(LogLevel::Warn, message.c_str(), caller);
}

int Logger::errorLine(const String &message, const char *caller) {
  return logLine(LogLevel::Error, message.c_str(), caller);
}

int Logger::fatalLine(const String &message, const char *caller) {
  return logLine(LogLevel::Fatal, message.c_str(), caller);
}
