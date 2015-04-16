#include <SoftwareSerial.h>
#include "Adafruit_NeoPixel.h"
#include "raycasting.h"
#include "readArea.h"
#include <Adafruit_GPS.h>

Adafruit_GPS GPS(&Serial1);
SoftwareSerial bluetooth(0,10); //RX,TX

int led = 7;

void sendData(vec pos);

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

vec *allowedArea;
polygon_t allowedAreap;
boolean hasAllowedArea = false;

char recievedChars[64];
float latitude, longitude;
vec recievedVectors[64];
boolean newData = false, readSerial = false, hasLat = false;
int numRecievedVectors = 0;

void loop()
{
	float fLat, fLon;
	//@TODO: Get polygon from web / Serial instead of hardcoding.
	//@TODO: Reads allowed area. Should check periodically if there's changes.
	//@TODO: BUG! Does not catch exeptions when there's a $ in the middle of a vector.

	//if (!hasAllowedArea){
		readArea();
	//}


	//Get current position by GPS fix                    
	char c = GPS.read();
	if (GPS.newNMEAreceived()) {
		//Serial.println(GPS.lastNMEA());
		if (!GPS.parse(GPS.lastNMEA()))
			return;
	}

	// if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	// approximately every 2 seconds check if current position is within the allowed area
	// @TODO: Initially wait a little more than 2 seconds to do the check, and if outside make the timer smaller.
	if (millis() - timer > 2000) {
		timer = millis(); // reset the timer
		if (GPS.fix) {
			//GPS Has been fixed. Check position, and send to phone if needed
			//@TODO: The inside check should take into account the quality of the GPS signal
			//@TODO: Low signal warning to phone?
			fLat = (decimalDegrees(GPS.latitude, GPS.lat));
			fLon = (decimalDegrees(GPS.longitude, GPS.lon));
			vec pos = { fLat, fLon };
			if (inside(pos, &allowedAreap, 1e-01) == -1){
				//@TODO: Send warning to phone, and start pushing data
				//Pushing data trough bluetooth done!
				digitalWrite(led, HIGH);
				sendData(pos);
			}
			else{
				//@TODO: If previously outside designated area, send "safe" notification to phone
				//			Keep pushing data untill safe notification has been sent
				//Else do nothing
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

void sendData(vec pos){
	bluetooth.println(pos.x,10);
	bluetooth.println(pos.y,10);
}
