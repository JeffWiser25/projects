// C++ code
// Daryl Sweeney
// c22718249
// TU705/2
// Instrumentation project - wind sensor

#define LED_RED 2   // LED indicator pin
#define LDR A0      // LDR sensor pin

int countup = 0;  // Counter for windmill arm passes
bool prevState = false; // Tracks previous sensor state (false = bright, true = dark)

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LDR, INPUT);

  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(LDR); // Read the LDR sensor
  bool currentState = (sensorValue < 200); // Check if it sees black

  // Detect transition from bright to dark (leading edge)
  if (currentState && !prevState) {  
    countup++;  // Increment the counter
    digitalWrite(LED_RED, HIGH);
    delay(500);  // Short debounce delay
    digitalWrite(LED_RED, LOW);
  }

  prevState = currentState;  // Update previous state

  // Print the number of times the windmill arm passed
  Serial.print("How many passes: ");
  Serial.println(countup);

  delay(50); // Small delay to avoid excessive CPU usage
}
