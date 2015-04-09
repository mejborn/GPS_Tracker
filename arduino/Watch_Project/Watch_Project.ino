#include <SoftwareSerial.h>
#include "Adafruit_NeoPixel.h"
#include "raycasting.h"
#include <Adafruit_GPS.h>

Adafruit_GPS GPS(&Serial1);
SoftwareSerial bluetooth(0,10); //RX,TX

int led = 7;

void setup()
{
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	GPS.begin(9600);
	bluetooth.begin(9600);
	// Turn on RMC (recommended minimum) and GGA (fix data) including altitude
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	// Set the update rate to 1 Hz
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

	delay(1000);
	// Ask for firmware version
	Serial1.println(PMTK_Q_RELEASE);
	//@TODO: Get designated area

}

//@TODO: Get polygon from web / Serial instead of hardcoding.
uint32_t timer = millis();
vec building322[] = {
		{ 55.78354322750842, 12.518640272319317 },
		{ 55.78338636792162, 12.519528083503246 },
		{ 55.783258164905725, 12.519459687173367 },
		{ 55.783415779142246, 12.518569193780422 },
};

polygon_t building322p = { 4, building322 };

char recievedChars[32];
boolean newData = false;

void loop()
{
	float fLat, fLon;
	//@TODO: Get polygon from web / Serial instead of hardcoding.

	//Send data trough the serial port
	if (Serial.available() > 0 && newData == false) {
		static byte ndx = 0;
		char endMarker = '\n';
		char pointMarker = ',';
		int rc = Serial.read();

		if (rc != endMarker && rc != pointMarker) {
			//Add value to the char array untill pointmarker has been reached
			recievedChars[ndx] = rc;
			ndx++;
		}
		else if (rc != endMarker && rc == pointMarker){
			// @TODO: Create a new vector from the char array and reset the char array
			//ATM It's just sending it via bluetooth for testing purposes
			recievedChars[ndx] = '\0'; // terminate the string
			for (int i = 0; i < sizeof(recievedChars) / sizeof(recievedChars[0]); i++){
				Serial1.print(recievedChars[i]);
				Serial.print(recievedChars[i]);
			}
			Serial1.println();
			Serial.println();
			memset(recievedChars, 0, 32);
			ndx = 0;
		}
		else {
			// @TODO: Create the last vector from char array and build a new polygon from vectors.
			// Reset input data

			recievedChars[ndx] = '\0'; // terminate the string

			memset(recievedChars, 0, 32);
			ndx = 0;
			newData = true;
		}
	}

	//Get current position by GPS fix                    
	char c = GPS.read();
	if (GPS.newNMEAreceived()) {
		//Serial.println(GPS.lastNMEA());
		if (!GPS.parse(GPS.lastNMEA()))
			return;
	}

	// if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	// approximately every 2 seconds or so, print out the current stats
	if (millis() - timer > 2000) {
		timer = millis(); // reset the timer
		if (GPS.fix) {
			//GPS Has been fixed. Check position, and send to phone if needed
			fLat = (decimalDegrees(GPS.latitude, GPS.lat));
			fLon = (decimalDegrees(GPS.longitude, GPS.lon));
			Serial.println(fLat, 6);
			Serial.println(fLon, 6);
			vec pos = { fLat, fLon };
			if (inside(pos, &building322p, 1e-01) == -1 && GPS.fix){
				//@TODO
				//I Am outside of the designated area. Send warning to phone!
				digitalWrite(led, HIGH);
				sendData(fLat, fLon);
			}
			else{
				//I Am inside of the designated area. Do nothing
				digitalWrite(led, LOW);
			}
		}
		else{
			//@TODO
			//Check how long it's been since I've had a gps fix.
			//If timer > 5 Minutes. Send warning to phone!
		}
	}
}

// Convert NMEA coordinate to decimal degrees
float decimalDegrees(float nmeaCoord, char dir) {
	uint32_t wholeDegrees = 0.01*nmeaCoord;
	int modifier = 1;

	if (dir == 'W' || dir == 'S') {
		modifier = -1;
	}

	return (wholeDegrees + (nmeaCoord - 100.0*wholeDegrees) / 60.0) * modifier;
}

void sendData(float latitude, float longitude){
	bluetooth.println(latitude);
	bluetooth.println(longitude);
}
