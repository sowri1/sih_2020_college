#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial s(5,6);

volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 5; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
int valve=6;
bool motor_on = 1;
bool fb_on = 1; // Changes on input from app
double supply_flow = 0;

void flow () // Interrupt function
{
   flow_frequency++;
}


void rate(){
  float a=0;  
  int count=0;
  float sensor=0;
 
  currentTime = millis();
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime;
      l_hour = (flow_frequency * 60 / 7.5);
      sensor += analogRead(A1);
      a=a+l_hour;
      count++;
      flow_frequency = 0;
   }
   float  sensorValue = sensor / count; 
   float voltage = sensorValue * (5.000 / 1023.000);
   float turbidity = 100.00 - (voltage / 2.85) * 100.00;
   Serial.print(turbidity);
   s.write(turbidity);   
   s.write("|");
   float average_rate=a/count;
   Serial.print("|");
   Serial.println(average_rate);
   s.write(average_rate);
   supply_flow += average_rate * 5 / 3.6;
   if(supply_flow > 2000){
     fb_on = 0;
     Serial.println("Limit reached");
   }
}
unsigned long main_start;


void setup()
{
  s.begin(9600);
  pinMode(flowsensor, INPUT);
   pinMode(A1,INPUT);
   digitalWrite(flowsensor, HIGH);
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING);
   sei();
   main_start = millis();
   currentTime = millis();
   // Get data from firebase about fb_on;  whether to on or off
   cloopTime = currentTime;
//   sw.begin(9600);
}

void do_motor_regulation(){
  if(fb_on){
    digitalWrite(valve, HIGH);
  }
  else{
    digitalWrite(valve, LOW);
    // Send notification to firebase;    
  }
}

void loop ()
{
  do_motor_regulation();
  if(fb_on==1){
    rate();
  }
  s.write("Hello world");
  if(millis()-main_start > 100000){
    Serial.println("Completed day");
    s.write(supply_flow);
    fb_on = 0;
    supply_flow = 0;
  }
//  Serial.println(Serial.read());
  delay(2000);
}
