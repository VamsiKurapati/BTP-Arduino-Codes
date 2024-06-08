#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const int oneWireBus = 4; 
const int ldrPin = 2; // Analog pin for LDR
const int darkValue = 800;  // Analog reading in darkness
const int brightValue = 200;  // Analog reading in bright light

const int voltageSensor = 32;
const int voltageSensor1 = 33;
const int voltageSensor2 = 34;
const int voltageSensor3 = 35;

const char *ssid = "Vamsi";
const char *pass = "Vamsi@1432";

const char *serverUrl = "http://192.168.199.44:5000/";

float R1 = 30000.0;
float R2 = 7500.0;
float value = 0.00;
float value1 = 0.00;
float value2 = 0.00;
float value3 = 0.00;

WiFiClient client;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup(){
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  WiFi.mode(WIFI_STA);
  sensors.begin();
}

void loop(){
  int ldrValue = analogRead(ldrPin); // Read analog value from LDR
  int lightIntensity = map(ldrValue, darkValue, brightValue, 0, 100);
  lightIntensity = constrain(lightIntensity, 0, 100);
  Serial.print("Intensity: ");
  Serial.println(lightIntensity);
  delay(200);
  
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature(C) = ");
  Serial.println(tempC);
  delay(200);

  value = analogRead(voltageSensor);
  value = (value * 5.0) / 3724.0;
  value = value / (R2 / (R1 + R2));
  Serial.print("Voltage = ");
  Serial.print(value);

  value1 = analogRead(voltageSensor1);
  value1 = (value1 * 5.0) / 3724.0;
  value1 = value1 / (R2 / (R1 + R2));
  Serial.print("V   Voltage1 = ");
  Serial.print(value1);
  
  value2 = analogRead(voltageSensor2);
  value2 = (value2 * 5.0) / 3724.0;
  value2 = value2 / (R2 / (R1 + R2));
  Serial.print("V   Voltage2 = ");
  Serial.print(value2);
  
  value3 = analogRead(voltageSensor3);
  value3 = (value3 * 5.0) / 3724.0;
  value3 = value3 / (R2 / (R1 + R2));
  Serial.print("V   Voltage3 = ");
  Serial.print(value3);
  Serial.println("V");

  sendToServer(tempC,lightIntensity,value,value1,value2,value3);
  
  client.stop();

  delay(10000);
}

void sendToServer(float tempC,int lightIntensity,float value,float value1,float value2,float value3) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client,serverUrl);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(200);
  doc["sensor_value"] = tempC;
  doc["Intensity"] = lightIntensity;
  doc["Voltage R1"] = value;
  doc["Voltage R2"] = value1;
  doc["Voltage C1"] = value2;
  doc["Voltage C2"] = value3;
  
  String requestBody;
  serializeJson(doc, requestBody);

  int httpCode = http.POST(requestBody);
  Serial.println(httpCode);
  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
