void wifi_check(void) {
  if (WiFi.status() == WL_NO_MODULE) {
    (void)logger.fatal("WiFi module not found. Halting.");
    while (true)
      ;
  }

  String firmware_ver = WiFi.firmwareVersion();
  if (firmware_ver < "1.0.0") {
    (void)logger.warning(F("WiFi module firmware is outdated."));
  }
}

void wifi_connect(void) {
  while (status != WL_CONNECTED) {
    (void)logger.logHeader(LogLevel::Info);
    (void)Serial.print(F("Attempting to connect to SSID: "));
    (void)Serial.println(SSID);
    status = WiFi.begin(SSID, PASS);
    delay(CONNECT_REATTEMPT_DELAY * 1000);
  }
}

void wifi_printStatus(void) {
  (void)logger.logHeader(LogLevel::Info);
  (void)Serial.print(F("SSID: "));
  (void)Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  (void)logger.logHeader(LogLevel::Info);
  (void)Serial.print(F("IP Address: "));
  (void)Serial.println(ip);

  (void)logger.logHeader(LogLevel::Info);
  (void)Serial.print(F("Signal Strength (RSSI): "));
  (void)Serial.print(WiFi.RSSI());
  (void)Serial.println(F(" dBm"));

  (void)logger.logHeader(LogLevel::Info);
  (void)Serial.print("To see this page in action, open a browser to http://");
  (void)Serial.println(ip);
}
