# PrettyLogger

Simple Arduino logging library with basic formatting options.

## Installation

You can either move this folder to your Arduino user libraries folder (`~/Documents/Arduino/libraries/` by default):

```pwsh
Move-Item -r plogger $HOME/Documents/Arduino/libraries/
```

Or create a Symlink at the same folder:

```pwsh
New-Item -Path $HOME/Documents/Arduino/libraries -ItemType SymbolicLink -Value plogger
```

## Sample Usage

```arduino
#include <plogger.h>

Logger logger(&Serial);
Ansi ansi(&Serial);

void setup(void) {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  logger.infoLine("Serial port connected.");
}

void loop(void) {
  data = Serial.readStringUntil('\r');
  if (data && data.length() > 0) {
    logger.infoLine("Received data.");

    ansi.GreenFg();
    Serial.println(data);
    ansi.Reset();
  }
}
```
