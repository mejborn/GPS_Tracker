void readArea(char data[]){
	bool hasLat = false;
	float longitude, latitude;
	for (int i = 0; i < sizeof(data) / sizeof(*data); i++){
		static byte ndx = 0;
		static char pointMarker = ',';
		int rc = data[i];


		//If serial reads $ and there hasnt been set any vectors, start reading vectors.
		//Else if serial reads $ and vectors has been set, input all the vectors into allowed area
		if (rc == '$'){
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

		}
		//Dont read null lines, or the $ sign
			if (rc == -1 || rc == 36){}
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
					}

					memset(recievedChars, 0, 32);
					ndx = 0;
				}
			}
		rc = (-1);
	}
}