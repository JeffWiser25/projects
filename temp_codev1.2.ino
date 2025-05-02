//Tu705/2
//group B
//version 1.2
//adapted by Daryl Sweeney,Adam Byrne,Ciaran Ryan
//c22718249
//12/02/2025
//class code originally written by gavin duffy
// we adjusted the m and c value to tune the circuit
//


int analogPin = A3;  // potentiometer wiper (middle terminal) connected to analog pin 3
                     // outside leads to ground and +5V
int val = 0;         // variable to store the value read
float m = 13.146;    //slope(m)
float c = 449.8;     //line intersection
float T = 0.0;       //Temperature set

void setup() {
  Serial.begin(9600);  //  setup serial
}

void loop() {
  val = analogRead(analogPin);  // read the input pin
  Serial.println(val);          // debug value
  //Rescale y = mx + c
  //A/D = mT + c
  //T = (A/D - c)/m
  T = (val - c) / m;
  Serial.print("Temperature =  ");

  Serial.print(T);
  Serial.println("degrees Celsius");
  delay(1000);
}
