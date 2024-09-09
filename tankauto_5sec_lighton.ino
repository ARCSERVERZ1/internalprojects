#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>


const char* ssid = "Orion";
const char* password = "digitalkey";
const int D4 = 2;
const int ledPin = LED_BUILTIN;
int flag = 0;
 unsigned long int low_trigger = millis();
 unsigned long int high_trigger = millis();
unsigned long int high_counter = 0;
unsigned long int low_counter = 0;
int set_value = 800;
int xprint = 1;

const char* webhookUrl = "http://www.virtualsmarthome.xyz/url_routine_trigger/activate.php?trigger=bf8da485-0d74-400a-8a40-10dbd0bf15ee&token=229a5580-3d6d-4b6f-ade9-cd30f176311d&response=json";
WiFiClient client;

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
     pinMode(D4, INPUT);
    pinMode(ledPin, OUTPUT);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.print("Connected to WiFi");
    Serial.println(WL_CONNECTED);

      if (WiFi.status() == WL_CONNECTED) {
      Serial.println(" initialising client");
      HTTPClient http;
      }

//  http.begin(client, webhookUrl);
//  Serial.println("web hook trigger");
//    int httpResponseCode = http.GET();
//
//  if (httpResponseCode > 0) {
//      Serial.print("HTTP Response code: ");
//      Serial.println(httpResponseCode);
//      String payload = http.getString();   // Get the response payload
//      Serial.println(payload);             // Print the response payload
//    } else {
//      Serial.print("Error code: ");
//      Serial.println(httpResponseCode);
//    }
//    
//    http.end();  // Free resources
//  } else {
//    Serial.println("WiFi Disconnected");
//    delay(1000);
  

}

void loop() {
int pinState = analogRead(A0); // Replace D1 with the pin you are checking
// Serial.println(pinState);

if ( pinState > set_value && flag == 0 && (millis() - high_trigger > 1000)) {
  high_trigger = millis();
  digitalWrite(ledPin, LOW);
  low_counter = 0;
  high_counter = high_counter+1;
  if (xprint == 1){
      Serial.print("High counter : ");
      Serial.print(high_counter);
      Serial.print(" | Value : ");
      Serial.print(pinState);
      Serial.print(" | Wifi status : ");
      Serial.println(WiFi.status());
  }
  if (high_counter > 4) {
     if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connectrd only");
     }
     else{
      Serial.println("No luck");
       while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
          HTTPClient http;
          http.begin(client, webhookUrl);
          Serial.println("web hook trigger");
          int httpResponseCode = http.GET();
              if (httpResponseCode > 0) {
                  Serial.print("HTTP Response code: ");
                  Serial.println(httpResponseCode);
                  String payload = http.getString();   
                  Serial.println(payload);             
                } 
                else {
                  Serial.print("Error code: ");
                  Serial.println(httpResponseCode);
                }
        http.end();  // Free resources
     }
    activate_trigger();
    Serial.println("Signal Continiously high continous for 5 Sec");
    high_counter = 0;
    flag = 1;
  }
}
  
if ( pinState < set_value && (millis() - low_trigger > 1000)) {
  low_trigger = millis();
  digitalWrite(ledPin, HIGH);
  high_counter = 0;
  low_counter = low_counter+1;
  if (xprint == 1){
      Serial.print("Low counter : ");
      Serial.print(low_counter);
      Serial.print(" | Value : ");
      Serial.println(pinState);
   }
  if (low_counter > 4) {
    low_counter = 0;
    flag = 0;
  } 
  }



} // loop close

void activate_trigger(){
 Serial.print("Trigger Activate and wifi status is : ");
 Serial.println(WiFi.status());
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" initialising ckient");
    HTTPClient http;
    http.begin(client, webhookUrl);
    Serial.println("web hook trigger");
    int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();   
            Serial.println(payload);             
          } 
          else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
          }
   http.end();  // Free resources
  } 
  else {
   Serial.println("WiFi Disconnected");
   delay(1000);
  }



}







    
 
