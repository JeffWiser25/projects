const int LDR = A0;
float lightLevel = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(LDR);
  float voltage = sensorValue * (5.0 / 1023.0);
  lightLevel = 0.6532 * voltage + 4.5155;

  Serial.print("Analog: ");
  Serial.print(sensorValue);
  Serial.print("  Voltage: ");
  Serial.print(voltage);
  Serial.print(" V  Light Level: ");
  Serial.print(lightLevel);
  Serial.println(" lux (?)");

  delay(500);
}
