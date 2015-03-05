#include <SoftwareSerial.h>
#include "Adafruit_NeoPixel.h"
#include "raycasting.h"
#include <Adafruit_GPS.h>

Adafruit_GPS GPS(&Serial1);
int led = 7;

void setup()  
{
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  GPS.begin(9600);
  
  // Turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Set the update rate to 1 Hz
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  delay(1000);
  // Ask for firmware version
  Serial1.println(PMTK_Q_RELEASE);
}

uint32_t timer = millis();
vec building322[] = {
    {55.783519189571834, 12.519038915634155},
    {55.78359761917681, 12.518561482429504},
    {55.78337288777143, 12.518513202667236},
    {55.78322356887254, 12.519484162330627},
    {55.78346790859092, 12.519492208957672}
  };
vec building101[] = {
                    {55.78571968336849, 12.521989345550537},
                    {55.787191637257564, 12.522804737091064},
                    {55.78658838419277, 12.526216506958008},
                    {55.78512847311928, 12.525486946105957}
  };
polygon_t building101p = { 4, building101 };
polygon_t building322p = { 5, building322 };  

void loop()
{
  float fLat,fLon;
  
  //Get data for where the watch can be safely (hardcoded atm)
  
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
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
  
  if (GPS.fix) {
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 2); Serial.print(GPS.lat);
    Serial.print(", "); 
    Serial.print(GPS.longitude, 2); Serial.println(GPS.lon);
     
    fLat = decimalDegrees(GPS.latitude, GPS.lat);
    fLon = decimalDegrees(GPS.longitude, GPS.lon);
  }
  
  //Check if current position is within pre-defined safe-zone
  vec pos = { fLat , fLon };
  if(inside(pos, &building322p, 1e-14) == -1){
    digitalWrite(led,HIGH);
  }else{
    digitalWrite(led,LOW);
  } 
}

// Convert NMEA coordinate to decimal degrees
float decimalDegrees(float nmeaCoord, char dir) {
  uint16_t wholeDegrees = 0.01*nmeaCoord;
  int modifier = 1;
 
  if (dir == 'W' || dir == 'S') {
    modifier = -1;
  }
  
  return (wholeDegrees + (nmeaCoord - 100.0*wholeDegrees)/60.0) * modifier;
}

