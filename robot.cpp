// Robot control program for Johnny705 simulator
//
//Engineering computing 2.1
//Robosimo-assignment
//Name: Daryl Sweeney
//Student number: C22718249
//Course/yr: TU705/2
//group B
//Last-updated: 12/10/2024 - 10:31

// Motor control pins:
//   D2 : Left motor forward
//   D3 : Left motor reverse
//   D4 : Right motor forward
//   D5 : Right motor reverse

// Sensor input pins:
//   A0 : Front left colour sensor
//   A1 : Front right colour sensor
//   A2 : Back left colour sensor
//   A3 : Back right colour sensor

int greyCounter = 0;  // Initialize a counter for grey ramps

void setup()
{
    // Set motor control pins as digital outputs
    pinMode(2, OUTPUT); // left motor forward pin
    pinMode(3, OUTPUT); // left motor reverse pin
    pinMode(4, OUTPUT); // right motor forward pin
    pinMode(5, OUTPUT); // right motor reverse pin
	
	Serial.begin(9600); // Start serial communication with the machine to print the message
}

void loop()//going into a loop
{
    // Read analogue sensor values
    int leftfront = analogRead(A0);
    int rightfront = analogRead(A1);
    int leftback = analogRead(A2);
	int rightback = analogRead(A3);
	
    // Specified color ranges defined
    int BLACK_MIN = 0;
    int BLACK_MAX = 100;
    int GREY_MIN = 300;
    int GREY_MAX = 600;
    int GREEN_MIN = 450; 
    int GREEN_MAX = 550;
    int WHITE_MIN = 950;
    int WHITE_MAX = 1023;
	

	
	// Check for grey detection on either front sensor
    if ((leftfront >= GREY_MIN && leftfront <= GREY_MAX) && 
        (rightfront >= GREY_MIN && rightfront <= GREY_MAX))
    {
        // Increment the counter if grey is detected
        greyCounter++;

        // Add a delay to prevent too many counts in one pass over the grey ramp
        delay(1000);  // 1000 millisecond delay to accommodate for the ramps
    }

    // Move forward if both front sensors detect less than BLACK_MAX - the maximum value for black
    if ((leftfront <= BLACK_MAX && rightfront <= BLACK_MAX))
    {
			digitalWrite(2, HIGH); // Left motor forward
            digitalWrite(3, LOW);  // Left motor reverse
            digitalWrite(4, HIGH); // Right motor forward
            digitalWrite(5, LOW);  // Right motor reverse
	}
	// Move forward if front two sensors detect grey by being within the grey threshold
	else if ((leftfront >= GREY_MIN && rightfront >= GREY_MIN)&&(leftfront <= GREY_MAX && rightfront <= GREY_MAX))
    {
            digitalWrite(2, HIGH); // Left motor forward
            digitalWrite(3, LOW);  // Left motor reverse
            digitalWrite(4, HIGH); // Right motor forward
            digitalWrite(5, LOW);  // Right motor reverse
    }
    
    // Turn right if left front sensor detects green
    else if ((leftfront >= GREEN_MIN) && (leftfront <= GREEN_MAX))
    {
        digitalWrite(2, HIGH); // Left motor forward
        digitalWrite(3, LOW);  // Left motor reverse
        digitalWrite(4, LOW);  // Right motor stop
        digitalWrite(5, LOW);  // Right motor reverse
		delay(0);
    }
    // Turn left if right front sensor detects green
    else if ((rightfront >= GREEN_MIN) && (rightfront <= GREEN_MAX))
    {
        digitalWrite(2, LOW);  // Left motor 
        digitalWrite(3, LOW);  // Left motor reverse
        digitalWrite(4, HIGH); // Right motor forward
        digitalWrite(5, LOW);  // Right motor reverse
		delay(0);
    }
	//When leftfront/rightfront/leftback/rightback detect white for 225ms then stops
    else if ((leftfront >= WHITE_MIN && leftfront <= WHITE_MAX) &&
             (rightfront >= WHITE_MIN && rightfront <= WHITE_MAX) &&
             (leftback >= WHITE_MIN && leftback <= WHITE_MAX) &&
             (rightback >= WHITE_MIN && rightback <= WHITE_MAX))
    {
        digitalWrite(2, LOW); // Stop left motor
        digitalWrite(3, LOW);
        digitalWrite(4, LOW); // Stop right motor
        digitalWrite(5, LOW);
        delay(220); // Optional delay to ensure the car stops for a moment
		
		
		// Print the final message once using a for loop
        for (int i = 0; i < 1; i++)  // Loop runs once to print the message
        {
            Serial.print("The car went over ");
            Serial.print(greyCounter);
            Serial.println(" grey ramps in total!");
			
        }

        // Stop further execution
        while (1); //Infinite loop of nothing to stop further actions
		//because while true and nothing to specify it goes on forever without doing nothing and stops
    }
    //parks car in white box
}
