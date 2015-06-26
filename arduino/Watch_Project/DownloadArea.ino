bool downloadArea(){
	char data[350];
	uint16_t i = 0;
	uint16_t statuscode;
	int16_t length;
	char downloadurl[] = "sidestreet.dk/arduino/get.php?ID=1";

        uint8_t m = fona.getNetworkStatus();
	if (m == 1 || m == 5) {
              Serial.println(F("****"));
              Serial.println("GO!");
              if(!fona.HTTP_GET_start(downloadurl, &statuscode, (uint16_t *)&length)){
		Serial.println("Download Failed!");
                return false;
              }
  	while (length > 0) {
  		while (fona.available()) {
                  char c = fona.read();
                  Serial.write(c);
                  data[i++] = c;
                  length--;
                  if (!length) break;
            }
	}
        //fona.HTTP_GET_end();
	Serial.println(F("\n****"));
	readArea(data);	
	
  }return true;
}
