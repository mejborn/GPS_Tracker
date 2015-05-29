bool DownloadArea(){
	char url[] = "http://sidestreet.dk/arduino/get.php?ID=1";
	static byte ndx = 0;
	static char pointMarker = ',';
	bool newdata = false, hasLat = false;
	float latitude, longitude;
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
			ReadPoints(rc, &newdata, &hasLat , &latitude , &longitude , &ndx);
#endif

			length--;
			if (!length) return true;
		}
	}
}

void ReadPoints(int rc , bool *newdata, bool *hasLat, float *latitude , float *longitude , byte *ndx){
	if (rc == '$' && *newdata){
		//If hasLat, the last longitude has not been set. Set it, and continue.
		if (hasLat){
			longitude = atof(recievedChars);
			vec vector = { latitude, longitude };
			recievedVectors[numRecievedVectors] = vector;
			numRecievedVectors++;
			hasLat = false;
		}
		polygon_t tmp = { numRecievedVectors / 2, recievedVectors };
		allowedAreap = tmp;
		Serial.println("Allowed area set!");
		Serial.print("Number of vectors recieved: ");
		Serial.println(numRecievedVectors);
		hasAllowedArea = true;
		newData = false;
		}

	//Dont read null lines, or the $ sign
	else if (!(rc == -1 || rc == 36)){
		static char pointMarker = ',';
		if (rc != pointMarker) {
			//Add value to the char array untill pointmarker has been reached
			recievedChars[ndx] = rc;
			ndx++;
		}
		else{
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