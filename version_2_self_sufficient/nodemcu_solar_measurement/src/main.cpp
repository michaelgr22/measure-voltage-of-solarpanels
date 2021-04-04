#include <Arduino.h>
//#include <NTPClient.h>
#include <HTTPClient.h>
#include <esp_adc_cal.h>
#include <esp_bt_main.h>
#include <esp_bt.h>
#include <esp_wifi.h>
#include <esp_sleep.h>
#include <string.h>
#include "time.h"

#include ".wificredentials.cpp"

//#include <WiFiUdp.h>
//#include <WiFi.h>

void connectToWifi()
{
  WifiCredentials wificredentials;

  //esp_wifi_start();

  WiFi.begin(wificredentials.ssid, wificredentials.password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to Wifi");
}

void initTime()
{
  connectToWifi();

  const char *ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 3600;

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void enterSleepMode(const int istimeless23)
{
  WiFi.disconnect(true);
  delay(300);

  esp_bluedroid_disable();
  esp_bt_controller_disable();
  esp_wifi_stop();

  uint64_t sleeptime = 900000000; //900000000 microseconds = 15 minutes
  //uint64_t sleeptime = 120000000; //120000000 microseconds = 2 minutes

  if (istimeless23 == 0)
  {
    sleeptime = 13500000000; //13500000000 microseconds = 3,75 h
  }

  esp_sleep_enable_timer_wakeup(sleeptime);
  esp_light_sleep_start();
}

int isTimeLess23()
{
  struct tm currenttime;

  getLocalTime(&currenttime);

  const int currenthour = currenttime.tm_hour;
  Serial.println(currenthour);

  if (currenthour >= 23)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

/*
int isTimeLess23()
{
  const long utcOffsetInSeconds = 3600;
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

  timeClient.begin();
  delay(50);
  timeClient.update();
  delay(50);
  const int currenthour = timeClient.getHours();
  Serial.println(currenthour);

  if (currenthour >= 23)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}*/

float getVoltage(const int pin)
{
  const int ref_voltage = 1135;
  adc1_channel_t channelnum;

  switch (pin)
  {
  case (39):
    channelnum = ADC1_CHANNEL_3;
    break;

  case (34):
    channelnum = ADC1_CHANNEL_6;
    break;

  case (35):
    channelnum = ADC1_CHANNEL_7;
    break;

  case (32):
    channelnum = ADC1_CHANNEL_4;
    break;
  }

  esp_adc_cal_characteristics_t *adc_chars = new esp_adc_cal_characteristics_t;

  analogReadResolution(11);
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_11, ref_voltage, adc_chars);
  adc1_config_channel_atten(channelnum, ADC_ATTEN_DB_11);

  int voltage_mv = esp_adc_cal_raw_to_voltage(analogRead(pin), adc_chars);

  return voltage_mv / 1000.0;
}

int sendPostRequest(const float resistor_voltage, const float opencircuit_voltage)
{
  const char *servername = "http://kuberneteshome:30830/sendvoltage-to-postgres.php"; //todo ip
  HTTPClient http;

  http.begin(servername);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httprequestdata = "resistorvoltage=" + String(resistor_voltage, 2) + "&opencircuitvoltage=" + String(opencircuit_voltage, 2);
  Serial.println(httprequestdata);

  return http.POST(httprequestdata);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Start");
  initTime();
}

void loop()
{
  const int pin_resistor_voltage = 39;
  const int pin_opencircuit_voltage = 35;
  delay(300);
  connectToWifi();
  sendPostRequest(getVoltage(pin_resistor_voltage), getVoltage(pin_opencircuit_voltage));
  enterSleepMode(isTimeLess23());
}