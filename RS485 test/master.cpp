const int ledPin =  13; 
const int EnTxPin =  2;
int angulo = 180;
void setup() 
{ 
  Serial.begin(9600);
  Serial.setTimeout(100);
  pinMode(ledPin, OUTPUT);
  pinMode(EnTxPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  digitalWrite(EnTxPin, HIGH); //We set ourselves as the transmitter 
} 
 
void loop() 
{ 
   if (angulo == 180) angulo = 170; else angulo = 180;

  Serial.print(String(angulo) + "|");
  delay(300); 
  
} 