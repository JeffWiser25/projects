#define Sensor_PORT A0
float rhRead,rhsensorPinOut = 0;
float RH, rhvoltage;
void func_humidity(void);

void setup(){
  Serial.begin(9600);
  
}

void loop(){

  rhRead = analogRead(A0);
  func_humidity();

  Serial.print("RELATIVE HUMIDITY: ");
  Serial.print(RH);
  Serial.print("%");
  Serial.println();
  delay(1000);
}

void func_humidity(void){
  rhvoltage = (rhRead/512)*2.5;
  RH = ((rhvoltage/3.3)-0.1515)/0.00636;
}
