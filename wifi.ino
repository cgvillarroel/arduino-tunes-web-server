void wifi_check(void) {
  if (WiFi.status() == WL_NO_MODULE) {
    (void)logger.fatal("WiFi module not found. Halting.");
    while (true)
      ;
  }

  String firmware_ver = WiFi.firmwareVersion();
  if (firmware_ver < "1.0.0") {
    (void)logger.warn(F("WiFi module firmware is outdated."));
  }
}

void wifi_connect(void) {
  while (status != WL_CONNECTED) {
    (void)logger.info(F("Attempting to connect to SSID: "));
    (void)logger.infoLine(SSID);
    status = WiFi.begin(SSID, PASS);
    delay(CONNECT_REATTEMPT_DELAY * 1000);
  }
}

void wifi_printStatus(void) {
  (void)logger.info(F("SSID: "));
  (void)logger.infoLine(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  String ip_string = String(ip[0]) + F(".") + String(ip[1]) + F(".") +
                     String(ip[2]) + F(".") + String(ip[3]);

  (void)logger.info(F("IP Address: "));
  (void)logger.infoLine(ip_string);

  (void)logger.info(F("Signal Strength (RSSI): "));
  (void)logger.info(String(WiFi.RSSI()));
  (void)logger.infoLine(F(" dBm"));

  (void)logger.info("To see this page in action, open a browser to http://");
  (void)logger.infoLine(ip_string);
}
