void postData(float latitude, float longitude){
	
	uint16_t statuscode;
	int16_t length;
	
	char* first = "sidestreet.dk/arduino/GCM/sendLatLng.php?ID=1";
	char lat[50];
    char lng[50];
    dtostrf(latitude, 1, 5, lat);
	dtostrf(longitude, 1, 5, lng);
        
        
    char url[300];
    strcpy(url, first);
    strcat(url, "&lat=");
    strcat(url, lat);
    strcat(url, "&lng=");
    strcat(url, lng);
        
	if (hasBeenOutside){//Has allready told it was outside. Just send coodinates.
		strcat(url, "&msgType=1");
		if (!fona.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
			Serial.println("Posting coodinates Failed!");
			return;
		} 
        Serial.println("Sent Coordinates");	
        }
	else{
		strcat(url, "&msgType=2");
		if (!fona.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
			Serial.println("Posting coodinates Failed!");
			return;
		}hasBeenOutside = true;
        Serial.println("Sent notification Message");
      	}

	
}
