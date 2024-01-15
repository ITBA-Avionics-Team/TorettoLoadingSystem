const int ledPin =  13; 
const int EnTxPin =  37;
int angulo = 180;
HardwareSerial serial = HardwareSerial(2);
void setup() 
{ 
  
  serial.begin(9600, SERIAL_8N1, 13, 21);
  // Serial.setTimeout(100);
  pinMode(ledPin, OUTPUT);
  pinMode(EnTxPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  digitalWrite(EnTxPin, HIGH); //We set ourselves as the transmitter 
} 
 
void loop() 
{ 
   if (angulo == 180) angulo = 170; else angulo = 180;

  serial.print(String(angulo) + "|");
  delay(300); 
  
} 