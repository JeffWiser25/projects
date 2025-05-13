const int LDR = A0;  // Analog pin connected to LDR voltage divider
float lux = 0;
float m = -0.6532;
float c = 4.5155;
int R_fixed = 1000;
float V_out = 0;
float R_LDR;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(LDR);
  V_out = sensorValue * (5.0 / 1023.0);  // Convert ADC value to voltage

  R_LDR = R_fixed * (V_out / (5 - V_out));
  lux = pow(10,(log10(R_LDR) - c)/m);
  

  Serial.print("Analog: ");
  Serial.print(sensorValue);
  Serial.print(" V_out: ");
  Serial.print(V_out, 3);
  Serial.print(" V  Light Level: ");
  Serial.print(lux, 1);
  Serial.println(" lux");
  Serial.print(R_LDR, 1);
  Serial.println(" :resistance");


  delay(500);
}
