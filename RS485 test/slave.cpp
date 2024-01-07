#include <Servo.h> 
 
Servo myservo;
const int EnTxPin =  2; 

unsigned long last_millis = 0;

char buffer[10];
void setup() 
{ 
  Serial.begin(9600);  
  myservo.attach(9);  //Servo pin
  pinMode(EnTxPin, OUTPUT);
  digitalWrite(EnTxPin, LOW); // Set ourselves as the receiver
  last_millis = millis();
} 
 
void loop() 
{ 
  if (millis() - last_millis > 20) {
    if(Serial.available())
    {

      int angle_num_len = Serial.readBytesUntil('|', buffer, 5);
      int angle = String(buffer).substring(0, angle_num_len).toInt();
      myservo.write(angle);
    }
    last_millis = millis();
  }
  
} 