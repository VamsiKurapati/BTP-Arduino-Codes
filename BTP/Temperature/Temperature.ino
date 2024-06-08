#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 32; //A0

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
 
void setup() 
{
  Serial.begin(115200);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature(C) = ");
  Serial.println(tempC);
  delay(1500);
}
