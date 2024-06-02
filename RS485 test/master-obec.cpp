// #include <Servo.h> 
#include <HardwareSerial.h>
 
// Servo myservo;
const int ENABLE_TX_PIN =  18;
const int ENABLE_RX_PIN =  5;
const int LED_PIN = 2;

unsigned long last_millis = 0;

char buffer[10];
int angulo = 180;
// HardwareSerial serial = HardwareSerial(2);
void setup() 
{ 
  Serial2.begin(115200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200); 
  Serial.println("Starting"); 
  pinMode(ENABLE_TX_PIN, OUTPUT);
  pinMode(ENABLE_RX_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(ENABLE_TX_PIN, HIGH); // We are the transmitter
  digitalWrite(ENABLE_RX_PIN, HIGH); // We are the transmitter
  last_millis = millis();
} 
 
void loop() 
{ 
  // if (millis() - last_millis > 20) {
    // Serial.println("R");
    if (angulo == 180) angulo = 170; else angulo = 180;
    digitalWrite(LED_PIN, HIGH);
    Serial.print(String("Sending ") + String(angulo) + "|");
    Serial2.print(String(angulo) + "|");
    digitalWrite(LED_PIN, LOW);
    delay(1000); 
  // }
  
} 