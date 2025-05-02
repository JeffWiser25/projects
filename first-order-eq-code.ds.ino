//Instrumentation
//TU705/2
//Group B
//Written by Daryl Sweeney
//
//Weatherstation-Project:
//Temperature sensor-
//First order-Equation-Code.V.101
//
//last updated - 10/02/2025
//
//
//adapted by ChatGPT
//

//declaring variables
const int sensorPin = A0;    // LM741 output connected to A0
float Voltage_ref = 5.0;     // Reference voltage
float gain = 50.0;           // Amplifier gain
float R0 = 100.0;            // PT100 resistance at 0°C (100Ω for PT100)
float alpha = 0.00391;       // PT100 temp coefficient (per °C)
float Voltage_offset = 0.5;  // Adjust this based on calibration, maybe no need for it just in case?

void setup() {
    Serial.begin(9600);
}

void loop() {
    int adcValue = analogRead(sensorPin); // Read from ADC(A0 pin)
    float Vout = (adcValue / 1023.0) * Voltage_ref;  // Convert ADC to voltage
    float Voltage_bridge = Vout / gain;  // Reverse amplifier effect

    // Calculate PT100 resistance from the voltage
    float Rpt100 = (Voltage_bridge * (1000.0 + R0)) / (Voltage_ref - Voltage_bridge); // Solve for PT100 resistance

    // Convert to temperature using PT100 equation
    float temperature = (Rpt100 - R0) / (alpha * R0); // Temperature in Celsius

    Serial.print("Voltage: "); 
    Serial.print(Vout);
    Serial.print(" V, Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    delay(1000);
}
