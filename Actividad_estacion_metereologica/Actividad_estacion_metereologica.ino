#include "DHT.h"
#include <NewPing.h>
#include "sendValuesThingSpeak.h"
#include "sendValuesUbiDots.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
#define WATERPIN A5
#define LUMPIN 6
//#define SOUNDPIN A4
#define DISTTRIGPIN 8
#define DISTECHOPIN 9
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(DISTTRIGPIN, DISTECHOPIN, 200);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  dht.begin();
  startThingSepak();
  startUbiDots();
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);

  // INICIO DHT
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
  // FIN DHT

  // INICIO WATER
  int water = analogRead(WATERPIN);
  Serial.print("Water: ");
  float waterCalc = map(water, 0, 550, 0, 100);
  Serial.println(waterCalc);
  // FIN WATER

  // INICIO SOUND
  /* int sound = analogRead(SOUNDPIN);
    Serial.print("Sound: ");
    Serial.println(sound);*/
  // FIN SOUND

  // INICIO LUMINOSIDAD
  int lum = digitalRead(LUMPIN);
  Serial.print("Luminosidad: ");
  Serial.println(lum);
  // FIN LUMINOSIDAD

  // INICIO DISTANCIA 5V
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
  // FIN DISTANCIA

  Serial.println();
  sentValueToThingSpeak(h,t,hic,waterCalc,cm);
  sendValuesToUbiDots(h,t,hic,waterCalc,cm);
}
