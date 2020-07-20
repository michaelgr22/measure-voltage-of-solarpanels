#include <stdio.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

void setup() {
 Serial.begin(9600);
 ConnectToNetwork();
 delay(5000);
}

void loop() {
  char data[30] = "";
  size_t sizedata = sizeof(data)/sizeof(data[0]);
  ReadRecievedData(data, sizedata);
  
  if ((data != NULL) && (data[0] != '\0')) {
    SendPostRequest(data);
    delay(50000);  
  }  
}

void ConnectToNetwork(){

  const char* ssid     = "***";
  const char* password = "***";

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void ReadRecievedData(char* data, size_t sizedata){
  String incomingdata="";
  boolean StringReady = false;

  while (Serial.available()){
    incomingdata=Serial.readString();
    incomingdata.toCharArray(data, sizedata);
    StringReady=true;
  }
  Serial.println(data);
}

void SendPostRequest(char* data){
  const char* serverName = "http://<ip>:31630/write-post-to-postgres.php";

  //Copy String because strtok needs more space
  char cdata[50] = "";
  strncpy(cdata, data, strlen(data));
  
  char* voltage = strtok(cdata, ",");
  char* date = strtok(NULL, ",");

  char datepost[22];
  size_t sizedatepost = sizeof(datepost)/sizeof(datepost[0]);
  ModifyDateForPost(date, datepost, sizedatepost);
  
  String voltageS = String(voltage);
  String datepostS = String(datepost);

  Serial.println("voltage " + voltageS);
  Serial.println("date " + datepostS);

  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
       
    // Prepare your HTTP POST request data
    String httpRequestData = "voltage=" + voltageS + "&measure_time=" + datepostS;

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void ModifyDateForPost(char* date, char* datepost, size_t sizedatepost){
  strncpy(datepost, date, strlen(date));
  int pos = 10; //Position of whitespace
  char toInsert[4] = "%20";
  
  //shift elements forward
  for (int i = sizedatepost; i >= pos; i--){
    datepost[i] = datepost[i-2];
  }

  for (int i = 0; i < strlen(toInsert); i++){
    datepost[pos+i] = toInsert[i];
  }
  datepost[sizedatepost-1] = '\0';
}
