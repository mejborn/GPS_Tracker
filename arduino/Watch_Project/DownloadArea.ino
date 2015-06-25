bool downloadArea(){
	char data[400];
	int i = 0;
	uint16_t statuscode;
	int16_t length;
	char url[] = "http://sidestreet.dk/arduino/get.php?ID=1";
	if (!fona.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
		Serial.println("Download Failed!");
		return false;
	}
	Serial.println("Url found. Trying to download");
	while (length > 0) {
		while (fona.available()) {
			char c = fona.read();

			// Serial.write is too slow, we'll write directly to Serial register!
			#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
				loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
				UDR0 = c;
			#else
				Serial.write(c);
				data[i++] = c;
			#endif

			length--;
			if (!length) break;
		}
	}Serial.println();
	Serial.println("Done reading data.");
	readArea(data);
	Serial.println(F("\n****"));
	fona.HTTP_POST_end();
	return true;
}