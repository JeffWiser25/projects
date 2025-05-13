// Instrumentation Project
// Weather Station
// Windspeed.v1
// TU705/2
// Authors: Daryl Sweeney, Patrick Bongala, Ciaran Ryan, Adam Byrne
// Last updated: 30/03/2025


#define pin 7 // TCRT5000 photo-sensor

unsigned long duration; // Timed duration between revolutions
unsigned long RPS; // Revolutions per second
unsigned long KMPH; // Calculated Kilometers per hour
    void setup() {
      Serial.begin(9600); // Serial monitor initialised at 9600 baudrate
      pinMode(pin, INPUT); // pin set as INPUT
    }

    void loop() {
      duration = pulseIn(pin, HIGH); // Using the pulseIn command to count revolution duration
      Serial.println(duration); // Printing duration on serial monitor
      RPS = 1000000/duration; // RPS calculation
      Serial.println(RPS); // Printing RPS value on serial print
      KMPH = (1.6406*(RPS*RPS)-10.06); // KMPH calculation - assuming a quadratic relationship between KMPH and speed
      if (KMPH > 100000){
        Serial.println("This is the speed 0"); // If KMPH is too much print zero(error)
      }
      else if (KMPH >0){
      Serial.print("This is the Speed "); // Else print KMPH 
      Serial.println(KMPH);
    }
   
