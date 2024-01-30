#ifndef ANSI_H
#define ANSI_H

#include <Arduino.h>

enum class EraseMode { After, Before, All };

/// ANSI helper class
/// Contains definitions and functions for terminal interactions
class Ansi {
  Stream *stream_;

public:
  Ansi(HardwareSerial *);

  int Reset(void);

  int BlackFg(void);
  int RedFg(void);
  int GreenFg(void);
  int YellowFg(void);
  int BlueFg(void);
  int MagentaFg(void);
  int CyanFg(void);
  int WhiteFg(void);

  int BrightBlackFg(void);
  int BrightRedFg(void);
  int BrightGreenFg(void);
  int BrightYellowFg(void);
  int BrightBlueFg(void);
  int BrightMagentaFg(void);
  int BrightCyanFg(void);
  int BrightWhiteFg(void);

  int BlackBg(void);
  int RedBg(void);
  int GreenBg(void);
  int YellowBg(void);
  int BlueBg(void);
  int MagentaBg(void);
  int CyanBg(void);
  int WhiteBg(void);

  int BrightBlackBg(void);
  int BrightRedBg(void);
  int BrightGreenBg(void);
  int BrightYellowBg(void);
  int BrightBlueBg(void);
  int BrightMagentaBg(void);
  int BrightCyanBg(void);
  int BrightWhiteBg(void);

  int previousLine(unsigned int);
  int eraseDisplay(EraseMode);
};

#endif // ANSI_H
