#include "ansi.h"

Ansi::Ansi(HardwareSerial *serial) { stream_ = serial; }

int Ansi::Reset(void) { return stream_->print(F("\x1b[0m")); }

int Ansi::BlackFg() { return stream_->print(F("\x1b[30m")); }
int Ansi::RedFg() { return stream_->print(F("\x1b[31m")); }
int Ansi::GreenFg() { return stream_->print(F("\x1b[32m")); }
int Ansi::YellowFg() { return stream_->print(F("\x1b[33m")); }
int Ansi::BlueFg() { return stream_->print(F("\x1b[34m")); }
int Ansi::MagentaFg() { return stream_->print(F("\x1b[35m")); }
int Ansi::CyanFg() { return stream_->print(F("\x1b[36m")); }
int Ansi::WhiteFg() { return stream_->print(F("\x1b[37m")); }

int Ansi::BrightBlackFg() { return stream_->print(F("\x1b[90m")); }
int Ansi::BrightRedFg() { return stream_->print(F("\x1b[91m")); }
int Ansi::BrightGreenFg() { return stream_->print(F("\x1b[92m")); }
int Ansi::BrightYellowFg() { return stream_->print(F("\x1b[93m")); }
int Ansi::BrightBlueFg() { return stream_->print(F("\x1b[94m")); }
int Ansi::BrightMagentaFg() { return stream_->print(F("\x1b[95m")); }
int Ansi::BrightCyanFg() { return stream_->print(F("\x1b[96m")); }
int Ansi::BrightWhiteFg() { return stream_->print(F("\x1b[97m")); }

int Ansi::BlackBg() { return stream_->print(F("\x1b[40m")); }
int Ansi::RedBg() { return stream_->print(F("\x1b[41m")); }
int Ansi::GreenBg() { return stream_->print(F("\x1b[42m")); }
int Ansi::YellowBg() { return stream_->print(F("\x1b[43m")); }
int Ansi::BlueBg() { return stream_->print(F("\x1b[44m")); }
int Ansi::MagentaBg() { return stream_->print(F("\x1b[45m")); }
int Ansi::CyanBg() { return stream_->print(F("\x1b[46m")); }
int Ansi::WhiteBg() { return stream_->print(F("\x1b[47m")); }

int Ansi::BrightBlackBg() { return stream_->print(F("\x1b[100m")); }
int Ansi::BrightRedBg() { return stream_->print(F("\x1b[101m")); }
int Ansi::BrightGreenBg() { return stream_->print(F("\x1b[102m")); }
int Ansi::BrightYellowBg() { return stream_->print(F("\x1b[103m")); }
int Ansi::BrightBlueBg() { return stream_->print(F("\x1b[104m")); }
int Ansi::BrightMagentaBg() { return stream_->print(F("\x1b[105m")); }
int Ansi::BrightCyanBg() { return stream_->print(F("\x1b[106m")); }
int Ansi::BrightWhiteBg() { return stream_->print(F("\x1b[107m")); }

int Ansi::previousLine(unsigned int lines) {
  int printed = 0;
  printed += stream_->print(F("\x1b["));
  printed += stream_->print((int)lines);
  printed += stream_->print(F("F"));
  return printed;
}

int Ansi::eraseDisplay(EraseMode mode) {
  int printed = 0;
  printed += stream_->print(F("\x1b["));
  printed += stream_->print((int)mode);
  printed += stream_->print(F("J"));
  return printed;
}
