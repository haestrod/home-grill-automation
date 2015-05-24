#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
//#include <stdint.h>
//#include <stdbool.h>
//#include "driverlib.h"

static volatile uint16_t curADCResult;
static volatile float normalizedADCRes;

// your network name also called SSID
char ssid[] = "nsa";
// your network password
char password[] = "youcantrustthis";

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(50,62,217,1);  // numeric IP for Google (no DNS)
char server[] = "192.168.159.166";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void wificonnect() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  printWifiStatus();
}

void setup() {
  wificonnect();
}

void Upload(float CurrentValue) {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 8000)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    Serial.println("%%%%%%%%%%%%");
    client.print("GET /temp/insert/");
    Serial.print("GET /temp/insert/");
    client.print(CurrentValue, 15);
    Serial.print(CurrentValue, 15);
    client.println(" HTTP/1.1");
    Serial.println(" HTTP/1.1");
    client.println("Host: 192.168.159.166:8000");
    Serial.println("Host: 192.168.159.166:8000");
    client.println("User Agent: Energia/1.1");
    Serial.println("User Agent: Energia/1.1");
    //client.println("Content-Length: 0");
    //Serial.println("Content-Length: 0");
    client.println("Connection: close");
    Serial.println("Connection: close");
    client.println();
    Serial.println("%%%%%%%%%%%%");
  }
}

void loop() {
  curADCResult = analogRead(A14);
  normalizedADCRes = (curADCResult * 3.3) / 16384;

  Upload(normalizedADCRes);
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    //char c = client.read();
    //Serial.write(c);
    client.flush();
  }
  delay(10000);
  // if the server's disconnected, stop the client:
/*  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    wificonnect();
  }*/
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

