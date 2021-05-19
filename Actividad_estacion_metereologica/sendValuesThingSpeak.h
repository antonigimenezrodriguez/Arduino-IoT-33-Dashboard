#include <WiFiNINA.h>
#include "ThingSpeak.h"

#define HUMIDITY_CHART 1
#define TEMPERATURE_CHART 2
#define COMPUTE_HEAT_CHART 3
#define WATER_CHART 4
#define DISTANCE_CHART 5
#define GYROSCOPE_AXIS_X_CHART 6
#define GYROSCOPE_AXIS_Y_CHART 7
#define GYROSCOPE_AXIS_Z_CHART 8

char ssid[] = "MIWIFI_2G_HTsX";   // your network SSID (name)
char pass[] = "CuDUdEse";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = 1391870;
const char * myWriteAPIKey = "8DCRMRESR20051RL";

void startThingSepak() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();

  ThingSpeak.begin(client);  //Initialize ThingSpeak
}

void sentValueToThingSpeak(float humidity, float temperature, float computeHeat, float water, int distance, float X_out, float Y_out, float Z_out) {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(1000);
    }
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(HUMIDITY_CHART, humidity);
  ThingSpeak.setField(TEMPERATURE_CHART, temperature);
  ThingSpeak.setField(COMPUTE_HEAT_CHART, computeHeat);
  ThingSpeak.setField(WATER_CHART, water);
  ThingSpeak.setField(DISTANCE_CHART, distance);
  ThingSpeak.setField(GYROSCOPE_AXIS_X_CHART, X_out);
  ThingSpeak.setField(GYROSCOPE_AXIS_Y_CHART, Y_out);
  ThingSpeak.setField(GYROSCOPE_AXIS_Z_CHART, Z_out);


  // set the status
  ThingSpeak.setStatus("status");
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
