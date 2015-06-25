void postData(float latitude, float longitude){
	
	uint16_t statuscode;
	int16_t length;
	
	
	char* first = "http://sidestreet.dk/arduino/GCM/sendLatLng.php?ID=1&lat=";
	char* lat; 
	sprintf(lat, "%.4g", latitude);
	char* lng; 
	sprintf(lng, "%.4g", longitude);
	
	char* firstlat = (char*) malloc(strlen(first) + strlen(lat) + 7);
	strcpy(firstlat, first);
	strcpy(firstlat, lat);
	strcpy(firstlat, "&lng=");
	
	char* firstlng = (char*) malloc(strlen(firstlat) + strlen(lng) + 11);
	strcpy(firstlng, firstlat);
	strcpy(firstlng, lng);
	
	char data[80]; 
	if (hasBeenOutside){//Has allready told it was outside. Just send coodinates.
		strcpy(firstlng, "msgType=1");
		if (!fona.HTTP_GET_start(firstlng, &statuscode, (uint16_t *)&length)) {
			Serial.println("Posting coodinates Failed!");
			return;
		}
	}
	else{
		strcpy(firstlng, "msgType=2");
		if (!fona.HTTP_GET_start(firstlng, &statuscode, (uint16_t *)&length)) {
			Serial.println("Posting coodinates Failed!");
			return;
		}
	}
	
}
