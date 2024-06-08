#include <WiFi.h>
#include <ThingSpeak.h>

const char *ssid = "Vamsi";
const char *password = "Vamsi@1432";
const char *thingSpeakApiKey = "A5Z698OTYTNOUMY4";
const unsigned long channelId = 2357915;
//const int dhtPin = D4;


WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  float temperature = 25.0;
  float humidity = 50.0;

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int writeResult = ThingSpeak.writeFields(channelId, thingSpeakApiKey);

  if (writeResult == 200) {
    Serial.println("Write successful");
  } else {
    Serial.print("Write failed. HTTP error code: ");
    Serial.println(writeResult);
    Serial.println("Check your ThingSpeak channel and connection");
  }

  delay(15000);
}
