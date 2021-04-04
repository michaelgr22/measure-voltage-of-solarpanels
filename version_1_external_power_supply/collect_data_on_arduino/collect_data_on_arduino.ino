#include <Wire.h>
#include <DS3231.h>
#include <stdio.h>

#include <SoftwareSerial.h>
SoftwareSerial ESP8266(2,3); //RX, TX

DS3231 clock;
RTCDateTime dt;

void setup() {
  Serial.begin(9600);
  ESP8266.begin(9600);
  Serial.println("Start");

  Serial.println("Initialize DS3231");
  // Initialize DS3231
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__); 
   
  delay(2000);
}

void loop() {
  dt = clock.getDateTime();

  if (dt.second == 0)
  {
    char DateAndTimeString[20];
    sprintf(DateAndTimeString, "%4d-%02d-%02d %d:%02d:%02d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
    Serial.println(DateAndTimeString);
    
    char DatatoSend[30] = "";
    float voltage = getvoltage();
    Serial.println(voltage);
    
    sprintf(DatatoSend, "%d.%02ld,", (int)voltage, (long int)(voltage*10000)%10000);
    sprintf(DatatoSend + strlen(DatatoSend), "%s", DateAndTimeString);
    
    Serial.println(DatatoSend);
    ESP8266.write(DatatoSend);
    delay(50000);  
  }
}

float getvoltage(){
  int analogInput = A0;
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 30000.0; 
  float R2 = 7500.0; 
  int value = 0;

  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0; // see text
  vin = vout / (R2/(R1+R2)); 
  return vin;
}
