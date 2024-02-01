void web_logAndSend(const String &message) {
  (void)logger.debug(message);
  (void)client.print(message);
}

void web_logAndSendLine(const String &message) {
  (void)logger.debug(message);
  (void)client.println(message);
};

/// Parameters: whether to include the html form in the response
void web_reply(MusicDetails *music) {
  (void)logger.info(F("Responding to request."));
  (void)web_logAndSendLine(F("HTTP/1.1 200 OK"));

  (void)web_logAndSendLine(F("Content-type:text/html"));
  (void)web_logAndSendLine(F(""));

  // html buttons
  (void)web_logAndSendLine(F("<form>"));

  (void)web_logAndSendLine(F("<label for=\"title\">Title</label><br />"));
  (void)web_logAndSend(F("<input name=\"title\" type=\"text\" value=\""));
  (void)web_logAndSend(music->title);
  (void)web_logAndSendLine(F("\" /><br />"));

  (void)web_logAndSendLine(F("<label for=\"tempo\">"
                             "Tempo (BPM / beats per minute)"
                             "</label><br />"));

  (void)web_logAndSend(F("<input name=\"tempo\" type=\"number\" value=\""));
  (void)web_logAndSend(String(music->tempo));
  (void)web_logAndSendLine(F("\" /><br />"));

  (void)web_logAndSendLine(F("<label for=\"notes\">"));
  (void)web_logAndSend(F("Notes (16th notes)</label>"));
  (void)web_logAndSendLine(F("<br />"));
  (void)web_logAndSend(F("<input name=\"notes\" type=\"text\"value=\""));
  (void)web_logAndSend(music->notes);
  (void)web_logAndSendLine(F("\" /><br />"));
  (void)web_logAndSendLine(F("<button type=\"submit\">Play!</button>"));
  (void)web_logAndSendLine(F("</form><br />"));

  (void)web_logAndSendLine(F("<form>"
                             "<input name=\"title\" type=\"text\""
                             "value=\"The Lick\" style=\"display:none;\" />"
                             "<input name=\"tempo\" type=\"number\""
                             "value=\"80\" style=\"display:none;\" />"
                             "<input name=\"notes\" type=\"text\" value=\""
                             "D4E4F4G4E4 C4D4-\" style=\"display:none;\" />"
                             "<button type=\"submit\">The Lick</button>"
                             "</form><br />"));

  (void)web_logAndSendLine(F("<form>"
                             "<input name=\"title\" type=\"text\" value=\""
                             "Megalovania\" style=\"display:none;\" />"
                             "<input name=\"tempo\" type=\"number\""
                             "value=\"200\" style=\"display:none;\" />"
                             "<input name=\"notes\" type=\"text\" value=\""
                             "D4 D4 D5   A4\" style=\"display:none;\" />"
                             "<button type=\"submit\">Megalovania</button>"
                             "</form><br />"));

  (void)web_logAndSendLine(F("<form>"
                             "<input name=\"title\" type=\"text\" value=\""
                             "Shooting Stars\" style=\"display:none;\" />"
                             "<input name=\"tempo\" type=\"number\""
                             "value=\"160\" style=\"display:none;\" />"
                             "<input name=\"notes\" type=\"text\" value=\""
                             "Ds4---- Ds4 E4   B3 Gs3\""
                             "style=\"display:none;\" />"
                             "<button type=\"submit\">Shooting Stars</button>"
                             "</form><br />"));

  (void)web_logAndSendLine(F(""));
}

void web_parseQuery(QueryParserState *state) {
  switch (state->parse_state) {

  case ParseState::Start:
    if (state->current_line.length() == 6 &&
        state->current_line.equals("GET /?")) {
      state->current_token = F("");
      state->parse_state = ParseState::Query;
      (void)logger.debug("Parsing query");
    }

    if (state->current_line.length() > 6) {
      state->parse_state = ParseState::None;
      (void)logger.debug("Done parsing query");
    }
    return;

  case ParseState::Query:
    if (state->c != '=') {
      return;
    }

    if (state->current_token.equals("title=")) {
      state->current_token = F("");
      state->parse_state = ParseState::Title;
      (void)logger.debug("Parsing title");
    } else if (state->current_token.equals("tempo=")) {
      state->current_token = F("");
      state->parse_state = ParseState::Tempo;
      (void)logger.debug("Parsing tempo");
    } else if (state->current_token.equals("notes=")) {
      state->current_token = F("");
      state->parse_state = ParseState::Notes;
      (void)logger.debug("Parsing notes");
    };
    return;

  case ParseState::Title:
    if (state->c != '&' && state->c != ' ') {
      return;
    }

    state->music.title = String(state->current_token.c_str());
    state->music.title.replace('+', ' ');
    state->current_token = F("");

    if (state->c == '&') {
      state->parse_state = ParseState::Query;
    } else {
      state->parse_state = ParseState::Done;
    }
    return;

  case ParseState::Tempo:
    if (state->c != '&' && state->c != ' ') {
      return;
    }

    state->music.tempo = state->current_token.toInt();
    state->current_token = F("");

    if (state->c == '&') {
      state->parse_state = ParseState::Query;
    } else {
      state->parse_state = ParseState::Done;
    }
    return;

  case ParseState::Notes:
    if (state->c != '&' && state->c != ' ') {
      return;
    }

    state->music.notes = String(state->current_token.c_str());
    state->music.notes.replace('+', ' ');
    state->current_token = F("");

    if (state->c == '&') {
      state->parse_state = ParseState::Query;
    } else {
      state->parse_state = ParseState::Done;
    }
    return;

  case ParseState::Done:
  case ParseState::None:
    return;
  }
}

void web_processRequest() {
  QueryParserState state = {.c = ' ',
                            .parse_state = ParseState::Start,
                            .current_line = F(""),
                            .current_token = F(""),
                            .music = (MusicDetails){
                                .title = F("The Lick"),
                                .tempo = 80,
                                .notes = F("C4D4E4F4G4A4B4"),
                            }};

  (void)logger.debug(F("Request:"));

  while (client.connected()) {
    if (!client.available()) {
      break;
    }

    state.c = client.read();

    if (state.c == '\n') {
      if (state.current_line.length() > 0) {
        // if the current line has stuff, reset the line and keep going
        (void)logger.debug(state.current_line);
        state.current_line = F("");
      } else {
        // if there are 2 new lines in a row, it's the end of the request
        (void)logger.logHeader(LogLevel::Info);
        (void)Serial.print(F("Title: "));
        (void)Serial.println(state.music.title);

        (void)logger.logHeader(LogLevel::Info);
        (void)Serial.print(F("Tempo: "));
        (void)Serial.println(state.music.tempo);

        (void)logger.logHeader(LogLevel::Info);
        (void)Serial.print(F("Notes: "));
        (void)Serial.println(state.music.notes);

        web_reply(&state.music);

        // if there's a query, play it
        if (state.parse_state == ParseState::Done) {
          music_handleRequest(&state.music);
        }

        return;
      }
    } else if (state.c != '\r') {
      state.current_line += state.c;

      if (state.c != '&' && state.c != ' ') {
        state.current_token += state.c;
      }
    }

    web_parseQuery(&state);
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
