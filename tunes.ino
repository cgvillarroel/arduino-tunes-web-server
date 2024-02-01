#include <LiquidCrystal_I2C.h>
#include <WiFiNINA.h>
#include <plogger.h>

#define RED_PIN 10
#define GREEN_PIN 9
#define BLUE_PIN 6

#define AUDIO_PIN 8
#define CONNECT_REATTEMPT_DELAY 10

#define MAX_MELODY_LENGTH 64

enum class LedPower { Off = 0, Half = 10, Max = 20 };

enum class ParseState { Start, Query, Title, Tempo, Notes, Done, None };

typedef struct MusicDetails {
  String title;
  int tempo;
  String notes;
} MusicDetails;

typedef struct QueryParserState {
  char c;
  ParseState parse_state;
  const String current_line;
  String current_token;
  MusicDetails music;
} QueryParserState;

const char *SSID = "HG8145V5_218F0";
const char *PASS = "znjuQ5ku";
int status = WL_IDLE_STATUS;

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiServer server(80);
WiFiClient client = server.available();

Logger logger(&Serial);

void setup(void) {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial)
    ;
  (void)logger.setLogLevel(LogLevel::Info);
  (void)logger.infoLine(F("Serial port connected."));

  lcd.init();

  wifi_check();
  wifi_connect();

  (void)logger.infoLine(F("Starting server."));
  server.begin();
  wifi_printStatus();
}

void loop(void) {
  client = server.available();
  if (client) {
    web_run();
  }
}
