/********************************
   Libraries included
 *******************************/

#include <SPI.h>
#include <WiFiNINA.h>
#include <avr/dtostrf.h>

/********************************
   Constants and objects
 *******************************/

#define DEVICE_LABEL "arduino-nano-33"
#define TOKEN "BBFF-yXiNaxooNoGhpPR8GHg5wdsb6lz7Xl"

char const * VARIABLE_LABEL_1 = "sensor";
char const *SERVER = "industrial.api.ubidots.com";
//Replace the above line if you are an Educational user char const *SERVER="things.ubidots.com";

const int HTTPPORT = 443;
char const *AGENT = "Arduino Nano 33 IoT";
char const *HTTP_VERSION = " HTTP/1.1\r\n";
char const *VERSION = "1.0";
char const *PATH = "/api/v1.6/devices/";

char const * SSID_NAME = "MIWIFI_2G_HTsX"; // Put here your SSID name
char const * SSID_PASS = "CuDUdEse"; // Put here your password

int status = WL_IDLE_STATUS;

WiFiSSLClient sslClient;
//void sendSingleValueToUbiDots(String, float);

/********************************
   Auxiliar Functions
 *******************************/
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
void getResponseServer() {
  Serial.println(F("\nUbidots' Server response:\n"));
  while (sslClient.available()) {
    char c = sslClient.read();
    Serial.print(c); // Uncomment this line to visualize the response on the Serial Monitor
  }
}

void waitServer() {
  int timeout = 0;
  while (!sslClient.available() && timeout < 5000) {
    timeout++;
    delay(1);
    if (timeout >= 5000) {
      Serial.println(F("Error, max timeout reached"));
      break;
    }
  }
}

void sendData(char* payload) {
  int contentLength = strlen(payload);

  /* Connecting the client */
  if (sslClient.connect(SERVER, HTTPPORT)) {
    Serial.println("connected to server");

    sslClient.print(F("POST "));
    sslClient.print(PATH);
    sslClient.print(DEVICE_LABEL);
    sslClient.print(F("/"));
    sslClient.print(HTTP_VERSION);
    sslClient.print(F("Host: "));
    sslClient.print(SERVER);
    sslClient.print(F("\r\n"));
    sslClient.print(F("User-Agent: "));
    sslClient.print(AGENT);
    sslClient.print(F("\r\n"));
    sslClient.print(F("X-Auth-Token: "));
    sslClient.print(TOKEN);
    sslClient.print(F("\r\n"));
    sslClient.print(F("Connection: close\r\n"));
    sslClient.print(F("Content-Type: application/json\r\n"));
    sslClient.print(F("Content-Length: "));
    sslClient.print(contentLength);
    sslClient.print(F("\r\n\r\n"));
    sslClient.print(payload);
    sslClient.print(F("\r\n"));

    Serial.print(F("POST "));
    Serial.print(PATH);
    Serial.print(DEVICE_LABEL);
    Serial.print(F("/"));
    Serial.print(HTTP_VERSION);
    Serial.print(F("Host: "));
    Serial.print(SERVER);
    Serial.print(F("\r\n"));
    Serial.print(F("User-Agent: "));
    Serial.print(AGENT);
    Serial.print(F("\r\n"));
    Serial.print(F("X-Auth-Token: "));
    Serial.print(TOKEN);
    Serial.print(F("\r\n"));
    Serial.print(F("Connection: close\r\n"));
    Serial.print(F("Content-Type: application/json\r\n"));
    Serial.print(F("Content-Length: "));
    Serial.print(contentLength);
    Serial.print(F("\r\n\r\n"));
    Serial.print(payload);
    Serial.print(F("\r\n"));

    waitServer();
    getResponseServer();
  }

  /* Disconnecting the client */
  sslClient.stop();
}

/********************************
   Main Functions
 *******************************/

void startUbiDots() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID_NAME);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(SSID_NAME, SSID_PASS);
    // wait 10 seconds for connection:
    delay(1000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();
}

void sendValuesToUbiDots(float humidity, float temperature, float computeHeat, float water, int distance) {
  
  char payload[200];
  char str_val_humidity[30];
  char str_val_temperature[30];
  char str_val_computeHeat[30];
  char str_val_water[30];
  char str_val_distance[30];

  /*4 is the total lenght of number,maximun number accepted is 99.99*/
  dtostrf(humidity, 4, 2, str_val_humidity);
  dtostrf(temperature, 4, 2, str_val_temperature);
  dtostrf(computeHeat, 4, 2, str_val_computeHeat);
  dtostrf(water, 4, 2, str_val_water);
  dtostrf(distance, 4, 2, str_val_distance);
  
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"");
  sprintf(payload, "%s%s\":%s", payload, "humidity", str_val_humidity);
  sprintf(payload, "%s,\"", payload);
  sprintf(payload, "%s%s\":%s", payload, "temperature", str_val_temperature);
  sprintf(payload, "%s,\"", payload);
  sprintf(payload, "%s%s\":%s", payload, "computeHeat", str_val_computeHeat);
  sprintf(payload, "%s,\"", payload);
  sprintf(payload, "%s%s\":%s", payload, "water", str_val_water);
  sprintf(payload, "%s,\"", payload);
  sprintf(payload, "%s%s\":%s", payload, "distance", str_val_distance);
  sprintf(payload, "%s}", payload);

  Serial.println("--------------------------------------------------");
  Serial.print("Payload :");
  Serial.println(payload);
  Serial.println("--------------------------------------------------");


  //Send the payload to Ubidots
  sendData(payload);
  //delay(5000);
}
