const unsigned int notes[] = {
  440, // A
  494, // B
  262, // C
  294, // D
  330, // E
  349, // F
  392, // G
};

const unsigned int colors[][3] = {
	{MAX_BRIGHTNESS, 0, MAX_BRIGHTNESS},              // A
	{MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS}, // B
	{MAX_BRIGHTNESS, 0, 0},                           // C
	{MAX_BRIGHTNESS, MAX_BRIGHTNESS, 0},              // D
	{0, MAX_BRIGHTNESS, 0},                           // E
	{0, MAX_BRIGHTNESS, MAX_BRIGHTNESS},              // F
	{0, 0, MAX_BRIGHTNESS},                           // G
};

void music_playLick(void) {
	String lick = F("DEFGE CD ");
	for (int i = 0; i < lick.length(); ++i) {
		if (lick[i] == ' ') {
			delay(NOTE_LENGTH);
			continue;
		}

		const int note_idx = lick[i] - 'A';
		const int note = notes[note_idx];
		tone(AUDIO_PIN, note);

		analogWrite(RED_PIN, colors[note_idx][0]);
		analogWrite(GREEN_PIN, colors[note_idx][1]);
		analogWrite(BLUE_PIN, colors[note_idx][2]);

		delay(NOTE_LENGTH);
	}

	noTone(AUDIO_PIN);
	analogWrite(RED_PIN, 0);
	analogWrite(GREEN_PIN, 0);
	analogWrite(BLUE_PIN, 0);
}
