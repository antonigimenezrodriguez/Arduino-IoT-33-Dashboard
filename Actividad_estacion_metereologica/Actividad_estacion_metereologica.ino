#include "sensors.h"
#include "sendValuesThingSpeak.h"
#include "sendValuesUbiDots.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  startSensors();
  // startThingSepak();
  // startUbiDots();
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);

  float DHTValues[3];
  getDHTValues(DHTValues);
  float h = DHTValues[0];
  float t = DHTValues[1];
  float hic = DHTValues[2];
  
  float water = getWater();
  
  unsigned int cm = getDistance();
  
  float gyroscopeValues[3];
  getGyroscopeValues(gyroscopeValues);
  float X_out = gyroscopeValues[0];
  float Y_out = gyroscopeValues[1];
  float Z_out = gyroscopeValues[2];
  delay(1000);

  Serial.println();
  // sentValueToThingSpeak(h, t, hic, water, cm);
  // sendValuesToUbiDots(h, t, hic, water, cm);
}
