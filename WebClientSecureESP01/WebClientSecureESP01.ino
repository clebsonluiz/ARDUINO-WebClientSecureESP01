#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>

#include "WebSecrets.h"


const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASS;
const char *server_host = HOST_ADDRESS;
const char *api_key_name = API_KEY_NAME;
const char *api_key_value = API_KEY_VALUE;
const char *route = HOST_ROUTE;

const char *fingerprint = FINGERPRINT;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  WiFiClientSecure https;   
  
  https.setFingerprint(fingerprint);
  https.setTimeout(15000); // 15 Seconds
  delay(1000);

  Serial.print("[POST REQUEST AT [");
  Serial.print(server_host);
  Serial.print("]] [FINGERPRINT [");
  Serial.print(fingerprint);
  Serial.println("]]...");


  Serial.print("Try connecting");
  int retryCounter = 0; 
  while ((!https.connect(server_host, 443)) && (retryCounter < 30)) { // 30 trys of connection
    delay(100);
    Serial.print(".");
    retryCounter++;
  }
  if (r >= 30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected, starting request.");

    String content = "{\"Hello\": \"World!\"}";

//        https.print(String("POST ") + route+ " HTTP/1.1\r\n" +
//                     "Host: " + server_host + "\r\n" +
//                     api_key_name + ": " + api_key_value + "\r\n"
//                     "Content-Type: application/json"+ "\r\n" +
//                     "Content-Length: " + content.length() + "\r\n\r\n" +
//                     content + "\r\n" +
//                     "Connection: close\r\n\r\n");

    /**
       Didatic purpose, equivalent to use
     **/
    https.print("POST "); https.print(route); https.print(" HTTP/1.1\r\n");
    https.print("Host: "); https.print(server_host); https.print("\r\n");
    https.print(api_key_name); https.print(": "); https.print(api_key_value); https.print("\r\n");
    https.print("Content-Type: application/json"); https.print("\r\n");
    https.print("Content-Length: "); https.print(content.length()); https.print("\r\n\r\n");
    https.print(content); https.print("\r\n");
    https.print("Connection: close"); https.print("\r\n\r\n");

    Serial.println("request sent");

    while (https.connected()) {
      String line = https.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }

    Serial.println("Response payload >>>>");
    Serial.println();
    String line;
    while (https.available()) {
      line = https.readStringUntil('\n'); 
      Serial.println(line); //Print response
    }
    Serial.println();
    Serial.println("<<<<.");
    Serial.println("End of connection.");
  }

  delay(5000);  //POST content at every 5 seconds
}
