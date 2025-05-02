//Instrumentation:
// wind-sensor.v3
// c22718249,c20336326,c23425844,c23459992
// Daryl Sweeney, Patrick Bongala, Ciaran Ryan, Adam Byrne
// Last updated: 12/03/2025
//

// Define pin numbers
#define IR_SENS A0  // Assuming the IR sensor is connected to an analog pin
#define NUM_ARMS 3   // Number of blades/paddles on the windmill

// Windmill and physics variables
const float pi = 3.1415926;
const float radius = 0.1;  // Windmill radius in meters
const float circumference = 2 * pi * radius;//circumference of a circle

// Air properties
const float Cd = 1.0;   // Drag coefficient (typical for blunt objects)
const float rho = 1.225; // Air density (kg/m³)
float A;  // Cross-sectional area

// Timing and counting variables
unsigned long prevTime = 0;
int counter = 0; 

void setup() {
  pinMode(IR_SENS, INPUT);

  Serial.begin(9600); // Start serial communication
  A = pi * radius * radius; // Calculate windmill cross-section area
}

void loop() {
  int sensorValue = analogRead(IR_SENS); // Read IR sensor value

  if (sensorValue > 100) {  // If black isnt detected (windmill arm passes)
    counter++;
    delay(50);  // Debounce delay to prevent multiple counts per blade
  } 
  unsigned long currentTime = millis();
  float elapsedTime = (currentTime - prevTime) / 1000.0; // Convert to seconds

  if (elapsedTime >= 1.0) {  // Update every 1 second
    float rev_per_sec = (counter / (float)NUM_ARMS) / elapsedTime;  // Revolutions per second
    float wind_speed_mps = rev_per_sec * circumference;
    float wind_speed_kmph = wind_speed_mps * (3600.0 / 1000.0);

    // Calculate drag force
    float drag_force = 0.5 * Cd * rho * A * (wind_speed_mps * wind_speed_mps);

    // Empirical correction (you might need to adjust this factor experimentally)
    float correction_factor = 0.1 * drag_force; // Arbitrary factor to scale the effect
    float compensated_wind_speed = wind_speed_kmph - correction_factor;
    
    // Print wind speed
    Serial.print("Wind Speed (km/h): ");
    Serial.println(compensated_wind_speed);
    Serial.println(counter);
    
    

    counter = 0;  // Reset counter for next interval
    prevTime = currentTime;  // Update time
  }
}
