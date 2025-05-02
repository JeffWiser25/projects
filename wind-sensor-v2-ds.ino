// wind-sensor.v2
// c22718249
// Daryl Sweeney, Patrick Bongala, Ciaran Ryan, Adam Byrne
// Last updated: 11/03/2025

// Define pin numbers
#define IR_LED 1
#define IR_SENS A0  // Assuming the IR sensor is connected to an analog pin
#define RED_LED 5

// Windmill and physics variables
float pi = 3.1415926;
float radius = 0.8;
float circumference = 2 * pi * radius;
float friction_coef = 0.1;

// Drag force variables
float Cd = 1.0;   // Drag coefficient
float rho = 1.225; // Air density (kg/m³)
float A;          // Cross-sectional area
float drag_force;

// Timing and counting variables
unsigned long prevTime = 0;
int counter = 0; 

void setup() {
  pinMode(IR_LED, OUTPUT);
  pinMode(IR_SENS, INPUT);
  pinMode(RED_LED, OUTPUT);

  Serial.begin(9600); // Start serial communication
  A = pi * radius * radius; // Calculate area
}

void loop() {
  int sensorValue = analogRead(IR_SENS); // Read sensor value

  if (sensorValue < 200) {  // If black detected (windmill arm passes)
    counter++;
    digitalWrite(RED_LED, HIGH);  // Optional LED indicator
    delay(50);  // Debounce delay
  } else {
    digitalWrite(RED_LED, LOW);
  }

  unsigned long currentTime = millis();//time starting when powered up
  float elapsedTime = (currentTime - prevTime) / 1000.0; // Convert to seconds

  if (elapsedTime >= 1.0) {  // Update every 1 second
    float rev_per_sec = (counter / 3.0) / elapsedTime;  // Revolutions per second
    float rev_per_min = rev_per_sec * 60;
    float wind_speed_mps = rev_per_sec * circumference;
    float wind_speed_kmph = wind_speed_mps * (3600 / 1000);

    // Calculate drag force
    float drag_force = 0.5 * Cd * rho * A * (wind_speed_mps * wind_speed_mps);
    float drag_force_kmph = drag_force * (3600 / 1000);
    //cd = 1.0, which is typical for blunt or irregular shapes.
    //at sea level, rho = 1.225 kg/m^3

    // Apply compensation
    float compensated_wind_speed = wind_speed_kmph - drag_force_kmph;

    // Print wind speed
    Serial.print("Wind Speed (km/h): ");
    Serial.println(compensated_wind_speed);

    counter = 0;  // Reset counter for next interval
    prevTime = currentTime;  // Update time
  }
}
