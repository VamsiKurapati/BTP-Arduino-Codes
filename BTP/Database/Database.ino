#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "Vamsi";
const char *password = "Vamsi@1432";
const char *serverUrl = "http://localhost:3000/"; //"http://192.168.148.44:3000/"; // Replace with your server endpoint

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  //int sensorValue = analogRead(A0);
  int sensorValue = 25;
  sendToServer(sensorValue);

  delay(10000); // Delay for 10 seconds before sending the next data
}

void sendToServer(int sensorValue) {
  WiFiClient client;
  HTTPClient http;
//  String url = String(serverUrl); // Replace with your specific endpoint on the server

  http.begin(client,serverUrl);
  http.addHeader("Content-Type", "application/json");

  // Customize the payload based on your data structure
  //  String payload = String(sensorValue);
  DynamicJsonDocument doc(200);
  doc["sensor_value"] = sensorValue;
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
