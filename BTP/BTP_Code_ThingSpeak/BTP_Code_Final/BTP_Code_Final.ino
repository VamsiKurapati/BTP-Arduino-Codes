#include <WiFi.h>
#include "ThingSpeak.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 2; //D4 for Temperature Sensor
const int voltageSensor = 35; //D5==14
const int voltageSensor1 = 36;  // T.B.D
const int voltageSensor2 = 39; // T.B.D
const int voltageSensor3 = 34; // T.B.D
const int ldrSensor = 13; //D7

float R1 = 30000.0;
float R2 = 7500.0;
float value = 0.0;
float value1 = 0.0;
float value2 = 0.0;
float value3 = 0.0;

const char* apiKey = "2YFJWFUEBVU7A9R5";
const char* ssid = "Vamsi";
const char* pass = "Vamsi@1432";
const char* server = "api.thingspeak.com";
 
WiFiClient client;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
 
void setup(){
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("WiFi connected");
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  sensors.begin();
}
 
void loop() {
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature(C) = ");
  Serial.print(tempC);
  delay(500);

  value = analogRead(voltageSensor);
  value = (value * 5.0) / 3724.0;
  value = value / (R2 / (R1 + R2));
  Serial.print("   Voltage = ");
  Serial.println(value);
  delay(500);

  value1 = analogRead(voltageSensor1);
  value1 = (value1 * 5.0) / 3724.0;
  value1 = value1 / (R2 / (R1 + R2));
  Serial.print("   Voltage1 = ");
  Serial.println(value1);
  delay(500);
  
  value2 = analogRead(voltageSensor2);
  value2 = (value2 * 5.0) / 3724.0;
  value2 = value2 / (R2 / (R1 + R2));
  Serial.print("   Voltage2 = ");
  Serial.println(value2);
  delay(500);
  
  value3 = analogRead(voltageSensor3);
  value3 = (value3 * 5.0) / 3724.0;
  value3 = value3 / (R2 / (R1 + R2));
  Serial.print("   Voltage3 = ");
  Serial.println(value3);
  delay(500);
  
  float LDR_Val = analogRead(ldrSensor);
  Serial.print("   Luminance : ");
  Serial.print(LDR_Val);
  delay(500);

  ThingSpeak.setField(1, tempC);
  ThingSpeak.setField(2, LDR_Val);
  ThingSpeak.setField(3, value);
  ThingSpeak.setField(4, value1);
  ThingSpeak.setField(5, value2);
  ThingSpeak.setField(6, value3);

  Serial.println("Sending....");
  int x = ThingSpeak.writeFields(1, apiKey);

  if(x == 200){
    Serial.println("Channel update successful.");
  }else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  client.stop();
  delay(2000);
}
