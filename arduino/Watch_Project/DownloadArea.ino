bool DownloadArea(){
	char url[] = "http://sidestreet.dk/arduino/get.php?ID=1";
	static byte ndx = 0;
	static char pointMarker = ',';
	bool readPoint = true;
	int rc;

	if (!fona.HTTP_POST_start(url, F("text/plain"), (uint8_t *)data, strlen(data), &statuscode, (uint16_t *)&length)) {
		Serial.println("Connection to URL failed!");
		return false;
	}
	while (length > 0) {
		while (fona.available()) {
			rc = fona.read();

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
			loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
			UDR0 = c;
#else
			if (readPoint){
				ReadPoint(rc);
			}
			else{

			}
			
			Serial.write(rc);
#endif

			length--;
			if (!length) return true;
		}
	}
}

void ReadPoint(int rc){
		//Dont read null lines, or the $ sign
	if (rc == -1 || rc == 36){
	}
	else{
		char c = rc;
		Serial.print(c);
		if (rc != pointMarker) {
			//Add value to the char array untill pointmarker has been reached
			recievedChars[ndx] = rc;
			ndx++;
		}
		else{
			Serial.println();
			recievedChars[ndx] = '\0'; // terminate the string
			if (!hasLat){
				latitude = atof(recievedChars);
				hasLat = true;
			}
			else{
				longitude = atof(recievedChars);
				vec vector = { latitude, longitude };
				recievedVectors[numRecievedVectors] = vector;
				numRecievedVectors++;
				hasLat = false;
				newData = true;
			}

			memset(recievedChars, 0, 32);
			ndx = 0;
		}
	}
}