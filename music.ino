const int note_colors[][3]{
    {(int)LedPower::Half, (int)LedPower::Off, (int)LedPower::Max}, // A4
    {(int)LedPower::Max, (int)LedPower::Off, (int)LedPower::Half}, // B4
    {(int)LedPower::Max, (int)LedPower::Off, (int)LedPower::Off},  // C4
    {(int)LedPower::Max, (int)LedPower::Max, (int)LedPower::Off},  // D4
    {(int)LedPower::Off, (int)LedPower::Max, (int)LedPower::Off},  // E4
    {(int)LedPower::Off, (int)LedPower::Max, (int)LedPower::Half}, // F4
    {(int)LedPower::Off, (int)LedPower::Half, (int)LedPower::Max}, // G4

    {(int)LedPower::Off, (int)LedPower::Off, (int)LedPower::Max},  // Ab4
    {(int)LedPower::Max, (int)LedPower::Off, (int)LedPower::Max},  // Bb4
    {(int)LedPower::Max, (int)LedPower::Off, (int)LedPower::Half}, // Cb4 (B3)
    {(int)LedPower::Max, (int)LedPower::Half, (int)LedPower::Off}, // Db4
    {(int)LedPower::Half, (int)LedPower::Max, (int)LedPower::Off}, // Eb4
    {(int)LedPower::Off, (int)LedPower::Max, (int)LedPower::Off},  // Fb4 (E4)
    {(int)LedPower::Off, (int)LedPower::Max, (int)LedPower::Max},  // Gb4

    {(int)LedPower::Max, (int)LedPower::Off, (int)LedPower::Max},  // A#4
    {(int)LedPower::Max, (int)LedPower::Off, (int)LedPower::Off},  // B#4 (C5)
    {(int)LedPower::Max, (int)LedPower::Half, (int)LedPower::Off}, // C#4
    {(int)LedPower::Half, (int)LedPower::Max, (int)LedPower::Off}, // D#4
    {(int)LedPower::Off, (int)LedPower::Max, (int)LedPower::Half}, // E#4 (F4)
    {(int)LedPower::Off, (int)LedPower::Max, (int)LedPower::Max},  // F#4
    {(int)LedPower::Off, (int)LedPower::Off, (int)LedPower::Max},  // G#4

    {(int)LedPower::Off, (int)LedPower::Off, (int)LedPower::Off} // Off
};

const int note_freqs[] = {
    440, // A4
    494, // B4
    262, // C4
    294, // D4
    330, // E4
    349, // F4
    392, // G4

    415, // Ab4
    466, // Bb4
    247, // Cb4 (B3)
    277, // Db4
    311, // Eb4
    330, // Fb4 (E4)
    370, // Gb4

    466, // A#4
    523, // B#4 (C5)
    277, // C#4
    311, // D#4
    349, // E#4 (F4)
    370, // F#4
    415, // G#4
};

void music_playMelody(int *melody, const int **colors, int melody_length,
                      int note_length) {
  if (melody_length > MAX_MELODY_LENGTH || melody_length == 0) {
    return;
  }

  for (int i = 0; i < melody_length; ++i) {
    analogWrite(RED_PIN, colors[i][0]);
    analogWrite(GREEN_PIN, colors[i][1]);
    analogWrite(BLUE_PIN, colors[i][2]);
    tone(AUDIO_PIN, melody[i]);

    (void)logger.logHeader(LogLevel::Debug);
    (void)Serial.print(F("Freq: "));
    (void)Serial.print(melody[i]);
    (void)Serial.print(F(" Red: "));
    (void)Serial.print(colors[i][0]);
    (void)Serial.print(F(" Green: "));
    (void)Serial.print(colors[i][1]);
    (void)Serial.print(F(" Blue: "));
    (void)Serial.println(colors[i][2]);

    delay(note_length);
  }

  noTone(AUDIO_PIN);
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}

/// Converts a String into an array of frequencies
/// String format: <note>[accidental (#|b)][octave]
/// music_parseString(String("DEFGE CD-"));
int music_parseString(const String &melody, int *frequencies, const int **colors) {
  int length = melody.length();
  // the longest representation of a note is 3 characters (e.g. C#4)
  int max_length = (MAX_MELODY_LENGTH * 3);
  if (length > max_length || length == 0) {
    return 0;
  }

  int idx = 0;
  int freq_idx = 0;
  int freq = 0;

  for (int i = 0; i < length; ++i) {
    char c = melody[i];

    if (c == 's') {
      freq_idx += 14;
      continue;
    }

    if (c == 'b') {
      freq_idx += 7;
      continue;
    }

    if (isAlpha(c)) {
      freq_idx = c - 'A';
      continue;
    }

    if (isDigit(c)) {
      double exponent = (double)((c - '0') - 4);
      double factor = pow(2, exponent);
      freq = note_freqs[freq_idx] * factor;
      frequencies[idx] = freq;
      colors[idx] = (const int *)note_colors[freq_idx];
      ++idx;
      continue;
    }

    if (c == '-') {
      frequencies[idx] = freq;
      colors[idx] = (const int *)note_colors[freq_idx];
      ++idx;
      continue;
    }

    if (c == ' ') {
      frequencies[idx] = 0;
      colors[idx] = (const int *)note_colors[21];
      ++idx;
      continue;
    }
  }

  return idx;
}

void music_handleRequest(MusicDetails *music) {
  (void)logger.logHeader(LogLevel::Info);
  (void)Serial.print(F("Playing: "));
  (void)Serial.println(music->title);

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Title:"));
  lcd.setCursor(0, 1);
  lcd.print(music->title);

  int frequencies[MAX_MELODY_LENGTH] = {0};
  const int *colors[MAX_MELODY_LENGTH];
  int length = music_parseString(music->notes, frequencies, colors);
  int note_length = (int)((double)15000 / (double)music->tempo);
  music_playMelody(frequencies, colors, length, note_length);

  delay(1000);
  lcd.clear();
  lcd.noBacklight();
}
