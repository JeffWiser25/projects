//formula-code.v1
//c22718249
//Daryl Sweeney, Patrick Bongala, Ciaran Ryan, Adam byrne
//last updated 11/03/2025
//
//



float distance = 1;
float pi = 3.1415926;
float r = 0.8;
float 1_rev = 2*pi*r;
float rev/s = distance/1_rev;
float rev/mn = rev/s*60;
float m/s = (rev/s)*(1_rev);
float km/hr = (m/s)*(1000/3600);
float friction_coef = 0.1;
float friction_force = (friction_coef)*(m/s);

//# Wind resistance (drag force) calculation
//# Assuming a drag coefficient (Cd) and air density (rho)
float Cd = 1.0;  //# drag coefficient (assumed)
float rho = 1.225;  //# air density in kg/m^3 (at sea level)
float A = pi * r^2;  //# cross-sectional area in m^2

//# Drag force formula: Fd = 0.5 * Cd * rho * A * v^2
float drag_force = 0.5 * Cd * rho * A * m/s^2;
float drag_force_km/hr = (drag_force)*(1000/3600);
float compensated_wf = km/hr - drag_force_km/hr;

#define IR_LED 1
#define IR_SENS 2
#define red_LED 5





void setup() {
  // put your setup code here, to run once:
  pinMode(IR_LED,OUTPUT) //IR_LED
  pinMode(IR_SENS,INPUT)  //IR_SENS

  Serial.begin(9600);//begin baud rate of 9600

  pinMode(red_LED, OUTPUT); // red LED counter


  

}

void loop() {
  int counter = 0;

  
  // put your main code here, to run repeatedly:
  Serial.println("Wind Force in km/ph:");
  Serial.println(" ");
  Serial.println(compensated_wf);

    if(IR_SENS < 200)//if IR_SENS detects black - an arm goes past
    {
      counter = counter + 1;
    }
    else
    (
      
    )
  

}
