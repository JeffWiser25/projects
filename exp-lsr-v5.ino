const int LDR = A0;  // Analog pin
int R_fixed = 1000;  // Fixed resistor in ohms
float V_out, R_LDR, lux;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(LDR);
  V_out = sensorValue * (5.0 / 1023.0);  // Convert ADC to voltage

  // Avoid division by zero
  if (V_out <= 0.0 || V_out >= 5.0) {
    Serial.println("V_out out of range");
    return;
  }

  // Assuming LDR is at the bottom (to GND)
  R_LDR = R_fixed * (V_out / (5.0 - V_out));

  // Rough lux estimation (for testing, update with proper curve later)
  lux = 500 / (R_LDR / 1000.0);  // e.g., 500kΩ = 1 lux, 5kΩ = 100 lux

  Serial.print("Analog: ");
  Serial.print(sensorValue);
  Serial.print(" | V_out: ");
  Serial.print(V_out, 3);
  Serial.print(" V | R_LDR: ");
  Serial.print(R_LDR, 1);
  Serial.print(" Ω | Light: ");
  Serial.print(lux, 1);
  Serial.println(" lux");

  delay(2500);
}
