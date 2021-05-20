#include "sensors.h"
#include "sendValuesThingSpeak.h"
#include "sendValuesUbiDots.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  startSensors();
  startThingSepak();
  startUbiDots();
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:

  float h = 0.00;
  float t = 0.00;
  float hic = 0.00;
  getDHTValues(h,t,hic);
  
  float water = getWater();
  
  unsigned int cm = getDistance();
  
  int X_out = 0;
  int Y_out = 0;
  int Z_out = 0;
  getGyroscopeValues(X_out, Y_out,Z_out);

  delay(2000);

  Serial.println();
  sentValueToThingSpeak(h, t, hic, water, cm, X_out, Y_out, Z_out);
  sendValuesToUbiDots(h, t, hic, water, cm, X_out, Y_out, Z_out);
}
