#include <SPI.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
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
ADXL345 adxl = ADXL345();



void startSensors() {

  Serial.begin(9600);
  Serial.println("Iniciando Sensores");
  Serial.println();
  dht.begin();
  adxl.powerOn();
  adxl.setRangeSetting(16);       //Definir el rango, valores 2, 4, 8 o 16
  Serial.println("Sensores iniciados correctamente");
}

void getDHTValues(float &hum, float &tem, float &comHeat ) {
  hum = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(hum);
  // Read temperature as Celsius (the default)
  tem = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(tem);
  // Compute heat index in Celsius (isFahreheit = false)
  comHeat = dht.computeHeatIndex(tem, hum, false);
  Serial.print("Compute Heat: ");
  Serial.println(comHeat);
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

void getGyroscopeValues(int &x, int &y, int &z) {
  
  adxl.readAccel(&x, &y, &z);

  Serial.print("Xa= ");
  Serial.print(x);
  Serial.print("   Ya= ");
  Serial.print(y);
  Serial.print("   Za= ");
  Serial.println(z);
}
