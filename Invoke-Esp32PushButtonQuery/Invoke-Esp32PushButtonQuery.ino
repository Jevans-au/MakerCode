/*
 * All references and help in the development of this code is referenced below.
 * The code comments I've (Jeff Evans - jevans.dev) added will be preceeded with a 'J - '
 * Values that need to be replaced will follow this format - YourBlankHere
Resource 1 in writing this code:
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

/*
Resource 2 in writing this code:
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>

// Replace with your network credentials
const char* ssid     = "YourWifiSsid";
const char* password = "YourWifiPassword";
const int buttonPin = 2;     // the number of the pushbutton pin
// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://YourHomeAssistantInstance:8123/api/services/light/toggle"; // J - I was testing this out with a simple light toggle using the Home Assistant API
int buttonState = 0;    

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);
  
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) { // J - In Resource 2 this is supposed to turn the light of the LED on, but in this instance I've made it the condition under which this query will occur.
      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      // J - Home Assistant API documentation covers the steps to get this token - https://developers.home-assistant.io/docs/api/rest
      http.addHeader("Authorization", "Bearer YourTokenHere");
      // Prepare your HTTP POST request data
      String httpRequestData = "{\"entity_id\":\"light.YourLightNameHere\"}";
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      String payload = http.getString();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      Serial.println(payload);
      // Free resources
      http.end();
      delay(500);  /* J - This delay was settled on after testing out some limits with my Home Assistant instance, I couldn't find a proper break point
                      J - but if you're finding there are issues with performance and responsiveness I recommend adding\subtracting time as required.
                      J - If not, hopefully you'll get the same performance as I did.
                   */
    }
        
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
