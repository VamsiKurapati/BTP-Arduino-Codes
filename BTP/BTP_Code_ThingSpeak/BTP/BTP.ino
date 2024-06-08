#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "ThingSpeak.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 2; //D4

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int voltageSensor = A0; //D5==14
const int ldrSensor = 13; //D7

float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
float value = 0.0;

const char* apiKey = "2YFJWFUEBVU7A9R5";
const char* ssid = "Vamsi";
const char* pass = "Vamsi@1432";
const char* server = "api.thingspeak.com";
 
WiFiClient client;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
 
void setup() 
{
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  delay(2000);
  lcd.clear();
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
  lcd.clear();
  lcd.print("Temp(C) : ");
  lcd.print(tempC);
  Serial.print("Temperature(C) = ");
  Serial.print(tempC);
  delay(1500);

  value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 3724.0;
  vIN = vOUT / (R2 / (R1 + R2));
  Serial.print("   Voltage = ");
  Serial.println(vIN);
  lcd.clear();
  lcd.print("Solar Volt:");
  lcd.print(vIN);
  delay(1500);
  
  float LDR_Val = analogRead(ldrSensor);
  Serial.print("   Luminance : ");
  Serial.print(LDR_Val);
  lcd.clear();
  lcd.print("Luminance : ");
  lcd.print(LDR_Val); 
  delay(1500);

  ThingSpeak.setField(1, vIN);
  ThingSpeak.setField(2, tempC);
  ThingSpeak.setField(3, LDR_Val);

  Serial.println("Sending....");
  int x = ThingSpeak.writeFields(1, apiKey);

  if(x == 200){
    Serial.println("Channel update successful.");
    lcd.clear();
    lcd.print("Data Transfering");
    lcd.setCursor(0,1);
    lcd.print("to ThingSpeak...");
  }else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  client.stop();
  delay(15000);
}
