void web_reply(void) {
  (void)logger.info(F("Responding to request."));
  (void)client.println(F("HTTP/1.1 200 OK"));
  (void)client.println(F("Content-type:text/html"));
  (void)client.println();

  // html buttons
  (void)client.println(F("Click <a href=\"/H\">here</a> turn the LED on<br>"));
  (void)client.println();

  (void)logger.debug(F("Reply:"));
  (void)logger.debug(F("HTTP/1.1 200 OK"));
  (void)logger.debug(F("Content-type:text/html"));
  (void)logger.debug(F(""));

  // html buttons
  (void)logger.debug(F("Click <a href=\"/H\">here</a> turn the LED on<br>"));
  (void)logger.debug(F(""));
}

bool web_processRequestLine(String &current_line) {
  if (!client.available()) {
    return false;
  }

  char c = client.read();

  if (c == '\n') {
    if (current_line.length() > 0) {
      (void)logger.debug(current_line);
      current_line = F("");
    } else {
      web_reply();
      return true;
    }
  } else if (c != '\r') {
    current_line += c;
  }

  if (current_line.endsWith("GET /H")) {
    (void)logger.info(F("Turning on LED."));
    music_playLick();
  }

  return false;
}

void web_run(void) {
  if (!client) {
    return;
  }

  String current_line = F("");
  if (client.connected()) {
    (void)logger.info(F("Client connected."));
    (void)logger.info(F("Receiving request."));
    (void)logger.debug(F("Request:"));
  }
  while (client.connected()) {
    if (web_processRequestLine(current_line)) {
      break;
    }
  }

  client.stop();
  (void)logger.info(F("Client disconnected."));
}
