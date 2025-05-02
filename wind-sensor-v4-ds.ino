// Instrumentation:
// wind-sensor.v4
// c22718249,c20336326,c23425844,c23459992
// Daryl Sweeney, Patrick Bongala, Ciaran Ryan, Adam Byrne
// Last updated: 12/03/2025
//

// Define pin numbers
#define IR_SENS A0  // Assuming the IR sensor is connected to an analog pin
#define NUM_ARMS 3   // Number of blades/paddles on the windmill

// Windmill and physics variables
const float pi = 3.1415926; //General Estimate of pi
const float radius = 0.1;  // Windmill radius in meters
const float circumference = 2 * pi * radius; // Circumference of a circle

// Air properties
const float Cd = 1.0;   // Drag coefficient (typical for blunt objects)
const float rho = 1.225; // Air density (kg/m³)
float A;  // Cross-sectional area

// Timing and counting variables
unsigned long prevTime = 0;
unsigned long lastBladeTime = 0;
int counter = 0; // Counts how many times the blade passes
bool prevBladeState = false;  // Stores previous blade detection state

void setup() {
  pinMode(IR_SENS, INPUT);

  Serial.begin(9600); // Start serial communication
  // # Increasing baud rate may quicken updating info on the serial monitor
  A = pi * radius * radius; // Calculate windmill cross-section area
}

void loop() {
  int sensorValue = analogRead(IR_SENS); // Read IR sensor value

  // Detect a blade pass (only count once per blade)
  bool bladeDetected = (sensorValue > 200); // Adjust threshold if needed

//   # If your blades are darker than the background, detecting black (sensorValue < 200) is correct.
//   or
//   # If your blades are lighter than the background, detecting not black (sensorValue > 200) could be better.

  

  if (bladeDetected && !prevBladeState) {  // If black detected (blade passes)
    counter++;
    lastBladeTime = millis();  // Store last detection time
  } 
  

  prevBladeState = bladeDetected;  // Store state for edge detection

  unsigned long currentTime = millis();
  float elapsedTime = (currentTime - prevTime) / 1000.0; // Convert to seconds

  if (elapsedTime >= 1.0) {  // Update every 1 second
    float rev_per_sec = (counter / (float)NUM_ARMS) / elapsedTime;  // Revolutions per second
    float wind_speed_mps = rev_per_sec * circumference;
    float wind_speed_kmph = wind_speed_mps * (3600.0 / 1000.0);

    // Calculate drag force
    float drag_force = 0.5 * Cd * rho * A * (wind_speed_mps * wind_speed_mps);

    // Empirical correction (adjust experimentally if needed)
    float correction_factor = 0.05 * drag_force;
    float compensated_wind_speed = wind_speed_kmph - correction_factor;

    // Correction_factor settings:
    // For Indoor/No wind - use: (0.05 * drag_force)
    // For Mild Spring day - use: (0.1 * drag_force)
    // For Windy Outdoor day - use: (0.15 * drag_force)
    // For Very Windy day - use: (0.2 * drag_force)
    
    // Print wind speed
    Serial.print("Wind Speed (km/h): ");
    Serial.println(compensated_wind_speed);
    Serial.print("Blade Count: ");
    Serial.println(counter);
    
    counter = 0;  // Reset counter for next interval
    prevTime = currentTime;  // Update time
  }
}
