#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 32; //A0 D32
const int ldrPin = 4;//D4

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
  delay(300);
  int ldrValue = analogRead(ldrPin); // Read analog value from LDR
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  delay(2000);
}
