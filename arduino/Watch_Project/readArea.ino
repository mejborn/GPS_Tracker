void readArea(char data[]){
	bool hasLat = false;
	float longitude, latitude;
        bool newData = false;
        Serial.println("Creating vectors");
        Serial.println(data);
        	for (int i = 0; i < 900; i++){
                static byte ndx = 0;
		static char pointMarker = ',';
		int rc = data[i];

		//If serial reads $ and there hasnt been set any vectors, start reading vectors.
		//Else if serial reads $ and vectors has been set, input all the vectors into allowed area
		if (rc == '$' && newData){
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
                        break;

		}
		//Dont read null lines, or the $ sign
	        else if (rc == -1 || rc == 36){}
	        else{
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
                                                newData = true;
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
