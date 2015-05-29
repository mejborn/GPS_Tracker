#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
#include "Adafruit_NeoPixel.h"
#include "raycasting.h"
#include "readArea.h"
#include <Adafruit_GPS.h>
#define FONA_RST 6
#define GPRSAPN "telenor.dk"
#define gprsrx 9
#define gprstx 10
#define deviceID "1" //Device ID for getting safe area

//Setup the GPS,Bluetooth, and GPRS ports
//Connect Ard pin D9 for RX, D10 for TX D6 for RST

Adafruit_GPS GPS(&Serial1);
SoftwareSerial bluetooth(0,10); //RX,TX
SoftwareSerial gprs(gprsrx, gprstx);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

int onModulePin = 12, aux;
int led = 7;
bool debug = true;
void sendData(vec pos);

void setup()
{
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	//bluetooth.begin(9600);

	// #####################
	// # Set up GPS Module #
	// #####################
	// Turn on RMC (recommended minimum) and GGA (fix data) including altitude
	GPS.begin(9600);
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	// Set the update rate to 1 Hz
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	delay(1000);
	// Ask for firmware version
	Serial1.println(PMTK_Q_RELEASE);
	delay(1000);

	// ######################
	// # Set up GPRS Module #
	// ######################
	gprs.begin(9600);
	// See if the FONA is responding
	fona.begin(gprs);
	fona.setGPRSNetworkSettings(F(GPRSAPN));
	fona.enableGPRS(true);
	//@TODO: Get designated area
	
}

//@TODO: Get polygon from web.
uint32_t timer = millis();

vec *allowedArea;
polygon_t allowedAreap;
boolean hasAllowedArea = false;

char recievedChars[64];
vec recievedVectors[64];
int numRecievedVectors = 0;

void loop()
{
	float fLat, fLon;
	//@TODO: Get polygon from web.
	//@TODO: Reads allowed area. Should check periodically if there's changes.
	//@TODO: BUG! Does not catch exeptions when there's a $ in the middle of a vector.

	//if (!hasAllowedArea){
	//	readArea();
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
		//Send GPS Debuggin info to Serial
		if (debug){
			Serial.print("\nTime: ");
			Serial.print(GPS.hour, DEC); Serial.print(':');
			Serial.print(GPS.minute, DEC); Serial.print(':');
			Serial.print(GPS.seconds, DEC); Serial.print('.');
			Serial.println(GPS.milliseconds);
			Serial.print("Date: ");
			Serial.print(GPS.day, DEC); Serial.print('/');
			Serial.print(GPS.month, DEC); Serial.print("/20");
			Serial.println(GPS.year, DEC);
			Serial.print("Fix: "); Serial.print((int)GPS.fix);
			Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
		}

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
