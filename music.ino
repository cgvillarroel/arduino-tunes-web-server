const unsigned int note_freqs[] = {
    440, // A4
    494, // B4
    262, // C4
    294, // D4
    330, // E4
    349, // F4
    392, // G4

    415, // Ab4
    466, // Bb4
    494, // Cb4 (B3)
    277, // Db4
    311, // Eb4
    330, // Fb4 (E4)
    370, // Gb4

    466, // A#4
    494, // B#4 (C5)
    277, // C#4
    311, // D#4
    349, // E#4 (F4)
    370, // F#4
    415, // G#4
};

void music_playMelody(unsigned int *melody, int melody_length,
                      unsigned int note_length) {
  if (melody_length > 16 || melody_length == 0) {
    return;
  }

  for (int i = 0; i < melody_length; ++i) {
		(void)logger.logHeader(LogLevel::Debug);
		(void)Serial.println(String(melody[i]));
    tone(AUDIO_PIN, melody[i]);
    delay(note_length);
  }

	noTone(AUDIO_PIN);
}

/// Converts a String into an array of frequencies
/// String format: <note>[accidental (#|b)][octave]
/// music_parseString(String("DEFGE CD-"));
int music_parseString(const String &melody,
                      unsigned int *frequencies) {
  int length = melody.length();
  int max_length = (MAX_MELODY_LENGTH * 3);
  if (length > max_length || length == 0) {
    return 0;
  }

  int idx = 0;
  int freqIdx = 0;
  unsigned int freq = 0;

  for (int i = 0; i < length; ++i) {
    char c = melody[i];

    if (c == '+') {
      frequencies[idx] = 0;
      ++idx;
      continue;
    }

    if (isAlpha(c)) {
      freqIdx = c - 'A';
      continue;
    }

    if (c == '#') {
      freqIdx += 7;
      continue;
    }

    if (c == 'b') {
      freqIdx += 14;
      continue;
    }

    if (isDigit(c)) {
      // double exponent = (double)((c - '0') - 4);
      freq = note_freqs[freqIdx];
      frequencies[idx] = freq;
      ++idx;
      continue;
    }

    if (c == '-') {
      frequencies[idx] = freq;
      ++idx;
      continue;
    }
  }

	return idx + 1;
}
