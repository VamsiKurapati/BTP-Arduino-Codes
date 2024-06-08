const int ldrPin = 2; // Analog pin for LDR
const int darkValue = 800;  // Analog reading in darkness
const int brightValue = 200;  // Analog reading in bright light

void setup() {
  Serial.begin(115200);
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Read analog value from LDR

  // Print raw analog reading to Serial Monitor
  Serial.print("Raw Analog Reading: ");
  Serial.println(ldrValue);

  // Map the analog reading to a light intensity range
  int lightIntensity = map(ldrValue, darkValue, brightValue, 0, 100);

  // Ensure the value is within bounds
  lightIntensity = constrain(lightIntensity, 0, 100);

  // Print calibrated light intensity to Serial Monitor
  Serial.print("Calibrated Light Intensity: ");
  Serial.println(lightIntensity);

  delay(1000); // Wait for a second
}
