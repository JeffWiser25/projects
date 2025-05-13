// Instrumentation:
// Wind-Sensor-no-LED-v5.1
// Authors: Daryl Sweeney, Patrick Bongala, Ciaran Ryan, Adam Byrne
// Last updated: 18/03/2025

// Define pin assignments
#define IR_SENS A0    // Infrared sensor input pin (analog)
#define NUM_ARMS 3    // Number of blades on the wind sensor

// Define physical properties of the wind sensor
const float pi = 3.1415926;  
const float radius = 0.1;  // Radius of the wind sensor (in meters)
const float circumference = 2 * pi * radius;  // Compute circumference (used for wind speed calculation)

// Define air properties for drag force calculations
const float Cd = 1.0;    // Drag coefficient (estimated for blunt objects)
const float rho = 1.225; // Air density (kg/m³)
float A;  // Cross-sectional area of the wind sensor (calculated in setup)

// Timing and counting variables
unsigned long prevTime = 0;       // Stores the last update time
unsigned long lastBladeTime = 0;  // Stores the last blade detection time
int counter = 0;                  // Counts blade passes
bool prevBladeState = false;      // Keeps track of the previous blade detection state

const int debounceTime = 50;  // Minimum time (ms) between blade detections to prevent false triggers

void setup() {
  pinMode(IR_SENS, INPUT);   // Set IR sensor pin as input

  Serial.begin(9600);  // Start serial communication at 115200 baud rate (for faster data transfer)

  A = pi * radius * radius;  // Compute the cross-sectional area of the wind sensor
}

void loop() {
  // Read the IR sensor value (detects whether a blade is passing)
  int sensorValue = analogRead(IR_SENS);
  bool bladeDetected = sensorValue;  // Blade detected if sensor value exceeds threshold (adjust if needed)

//   # If your blades are darker than the background, detecting black (sensorValue < 150) is correct.
//   or
//   # If your blades are lighter than the background, detecting not black (sensorValue > 150) could be better.


  // Check if a blade passes (only count it once using debounce time)
if (bladeDetected && !prevBladeState && millis() - lastBladeTime > debounceTime) {
    counter++;  // Increment blade count
    lastBladeTime = millis();  // Update last detection time
}


  prevBladeState = bladeDetected;  // Store current blade detection state for next iteration

  // Calculate wind speed every 0.5 seconds
  unsigned long currentTime = millis();
  float elapsedTime = (currentTime - prevTime) / 1000.0;  // Convert milliseconds to seconds 

  if (elapsedTime >= 0.5) { 
    float rev_per_sec = (counter / (float)NUM_ARMS) / elapsedTime;  // Calculate revolutions per second
    float wind_speed_mps = rev_per_sec * circumference;  // Compute wind speed in meters per second
    float wind_speed_kmph = wind_speed_mps * (3600.0 / 1000.0);  // Convert m/s to km/h

    // Compute drag force based on wind speed
    float drag_force = 0.5 * Cd * rho * A * (wind_speed_mps * wind_speed_mps);
    float correction_factor = 0.02 * drag_force;  // Apply correction factor for wind speed adjustment
    float compensated_wind_speed = wind_speed_kmph - correction_factor;  // Adjust wind speed

    // Correction_factor settings:
    // For Indoor/No wind - use: (0.05 * drag_force)
    // For Mild Spring day - use: (0.1 * drag_force)
    // For Windy Outdoor day - use: (0.15 * drag_force)
    // For Very Windy day - use: (0.2 * drag_force)

    // Print wind speed and blade count to Serial Monitor
    Serial.print("Wind Speed (km/h): ");
    Serial.println(compensated_wind_speed);
    Serial.print("Blade Count: ");
    Serial.println(counter);

    counter = 0;  // Reset counter for next interval
    prevTime = currentTime;  // Update last measurement time
  }
}
