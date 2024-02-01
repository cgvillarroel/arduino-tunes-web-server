void web_logAndSend(const String &message) {
  (void)logger.debug(message);
  (void)client.println(message);
};

void web_reply(void) {
  (void)logger.info(F("Responding to request."));
  (void)web_logAndSend(F("HTTP/1.1 200 OK"));
  (void)web_logAndSend(F("Content-type:text/html"));
  (void)web_logAndSend(F(""));

  // html buttons
  (void)web_logAndSend(F("<form>"));
  (void)web_logAndSend(F("<label for=\"title\">Title</label><br />"));
  (void)web_logAndSend(F("<input name=\"title\" type=\"text\""
                         "value=\"My Song\"/><br />"));
  (void)web_logAndSend(F("<label for=\"tempo\">"
                         "Tempo (BPM / beats per minute)"
                         "</label><br />"));
  (void)web_logAndSend(F("<input name=\"tempo\" type=\"number\""
                         "value=\"100\"/><br />"));
  (void)web_logAndSend(F("<label for=\"notes\">Notes</label><br />"));
  (void)web_logAndSend(F("<input name=\"notes\" type=\"text\""
                         "value=\"D4E4F4G4E4 C4D4-\" /><br />"));
  (void)web_logAndSend(F("<button type=\"submit\">Play!</button>"));
  (void)web_logAndSend(F("</form>"));
  (void)web_logAndSend(F(""));
}

void web_parseQuery(ParseState &parse_state, const String &current_line,
                    String &current_token, String &title, String &notes,
                    int *tempo, char c) {
  switch (parse_state) {

  case ParseState::Start:
    if (current_line.length() == 6 && current_line.endsWith("GET /?")) {
      current_token = F("");
      parse_state = ParseState::Query;
      (void)logger.debug("Parsing query");
    }

    if (current_line.length() > 6) {
      parse_state = ParseState::Done;
      (void)logger.debug("Done parsing query");
    }
    return;

  case ParseState::Query:
    if (c != '=') {
      return;
    }

    if (current_token.endsWith("title=")) {
      current_token = F("");
      parse_state = ParseState::Title;
      (void)logger.debug("Parsing title");
    } else if (current_token.endsWith("tempo=")) {
      current_token = F("");
      parse_state = ParseState::Tempo;
      (void)logger.debug("Parsing tempo");
    } else if (current_token.endsWith("notes=")) {
      current_token = F("");
      parse_state = ParseState::Notes;
      (void)logger.debug("Parsing notes");
    };
    return;

  case ParseState::Title:
    if (c != '&' && c != ' ') {
      return;
    }

    title = String(current_token.c_str());
    title.replace('+', ' ');
    current_token = F("");

    if (c == '&') {
      parse_state = ParseState::Query;
    } else {
      parse_state = ParseState::Done;
    }
    return;

  case ParseState::Tempo:
    if (c != '&' && c != ' ') {
      return;
    }

    *tempo = current_token.toInt();
    current_token = F("");

    if (c == '&') {
      parse_state = ParseState::Query;
    } else {
      parse_state = ParseState::Done;
    }
    return;

  case ParseState::Notes:
    if (c != '&' && c != ' ') {
      return;
    }

    notes = String(current_token.c_str());
    title.replace('+', ' ');
    notes.replace("%23", "#");
    current_token = F("");

    if (c == '&') {
      parse_state = ParseState::Query;
    } else {
      parse_state = ParseState::Done;
    }
    return;

  case ParseState::Done:
    return;
  }
}

bool web_processRequest() {
  ParseState parse_state = ParseState::Start;
  String current_line = F("");
  String current_token = F("");
  String title = F("");
  String notes = F("");
  int tempo = 0;

  (void)logger.debug(F("Request:"));

  while (client.connected()) {
    if (!client.available()) {
      break;
    }

    char c = client.read();

    if (c == '\n') {
      if (current_line.length() > 0) {
        // if the current line has stuff, reset the line and keep going
        (void)logger.debug(current_line);
        current_line = F("");
      } else {
        // if there are 2 new lines in a row, it's the end of the request
        (void)logger.logHeader(LogLevel::Info);
        (void)Serial.print(F("Title: "));
        (void)Serial.println(title);

        (void)logger.logHeader(LogLevel::Info);
        (void)Serial.print(F("Tempo: "));
        (void)Serial.println(tempo);

        (void)logger.logHeader(LogLevel::Info);
        (void)Serial.print(F("Notes: "));
        (void)Serial.println(notes);

        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print(F("Title:"));
        lcd.setCursor(0, 1);
        lcd.print(title);
        unsigned int frequencies[MAX_MELODY_LENGTH] = {0};
        int length = music_parseString(notes, frequencies);
        music_playMelody(frequencies, length,
                         (int)((double)15000 / (double)tempo));

        delay(1000);
        lcd.clear();
        lcd.noBacklight();

        web_reply();
        break;
      }
    } else if (c != '\r') {
      current_line += c;

      if (c != '&' && c != ' ') {
        current_token += c;
      }
    }

    web_parseQuery(parse_state, current_line, current_token, title, notes,
                   &tempo, c);
  }
}

void web_run(void) {
  if (!client) {
    return;
  }

  String current_line = F("");
  if (client.connected()) {
    (void)logger.info(F("Client connected."));
    (void)logger.info(F("Receiving request."));
    web_processRequest();
  }

  client.stop();
  (void)logger.info(F("Client disconnected."));
}
