#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
//#include <stdint.h>
//#include <stdbool.h>
//#include "driverlib.h"

static volatile uint16_t curADCResult;
//static volatile float normalizedADCRes;

// your network name also called SSID
char ssid[] = "nsa";
// your network password
char password[] = "youcantrustthis";

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,159,166);  // numeric IP for server (no DNS)
//char server[] = "192.168.159.166";    // name address for server (using DNS)

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

void Upload(int CurrentValue) {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 8000)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST /temp HTTP/1.1");
    client.println("Host: 192.168.159.166:8000");
    client.println("User Agent: Energia/1.1");
    client.print("Content-Length: ");
    int thisLength = 8 + getLength(CurrentValue);
    client.println(thisLength);
//    client.println("Content-Type: text/csv");
    client.println("Connection: close");
    client.println();
    client.print("sensor1=");
    client.println(CurrentValue);
    
    delay(300);
  }
}

void loop() {
  curADCResult = analogRead(A14);
//  normalizedADCRes = (curADCResult * 3.3) / 16384;

  Upload(curADCResult);
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

// This method calculates the number of digits in the
// sensor reading.  Since each digit of the ASCII decimal
// representation is a byte, the number of digits equals
// the number of bytes:

int getLength(int someValue) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten,
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue / 10;
  while (dividend > 0) {
    dividend = dividend / 10;
    digits++;
  }
  // return the number of digits:
  return digits;
}

