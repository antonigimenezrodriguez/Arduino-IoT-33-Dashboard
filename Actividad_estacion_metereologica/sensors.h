#include <Wire.h>  // Wire library - used for I2C communication
#include "DHT.h"
#include <NewPing.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
#define WATERPIN A7
#define DISTTRIGPIN 8
#define DISTECHOPIN 9
#define GYROSCLPIN A5
#define GYROSDAPIN A4
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(DISTTRIGPIN, DISTECHOPIN, 200);

int ADXL345 = 0x53; //60 6A The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs

void startSensors() {
  dht.begin();
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();
}

void getDHTValues(float gyroscopeValues[]) {
  float h = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(h);
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(t);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Compute Heat: ");
  Serial.println(hic);
  gyroscopeValues[0] = h;
  gyroscopeValues[1] = t;
  gyroscopeValues[2] = hic;
}


float getWater() {
  int water = analogRead(WATERPIN);
  float waterCalc = map(water, 0, 550, 0, 100);
  Serial.print("Water: ");
  Serial.println(waterCalc);
  return waterCalc;
}

unsigned int getDistance() {
  unsigned int cm = sonar.ping_cm();
  Serial.print("Distancia: ");
  Serial.print(cm); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  if (cm == 0 && digitalRead(DISTECHOPIN) == LOW) {
    pinMode(DISTECHOPIN, OUTPUT);
    digitalWrite(DISTECHOPIN, LOW);
    delay(100);
    pinMode(DISTECHOPIN, INPUT);
  }
  return cm;
}

void getGyroscopeValues(float gyroscopeValues[]) {
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read() | Wire.read() << 8); // X-axis value
  X_out = X_out / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read() | Wire.read() << 8); // Y-axis value
  Y_out = Y_out / 256;
  Z_out = ( Wire.read() | Wire.read() << 8); // Z-axis value
  Z_out = Z_out / 256;
  Serial.print("Xa= ");
  Serial.print(X_out);

  Serial.print("   Ya= ");
  Serial.print(Y_out);


  Serial.print("   Za= ");
  Serial.println(Z_out);
  gyroscopeValues[0] = X_out;
  gyroscopeValues[1] = Y_out;
  gyroscopeValues[2] = Z_out;
}
