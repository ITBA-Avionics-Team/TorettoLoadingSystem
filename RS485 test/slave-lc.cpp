// #include <Servo.h>
#include <HardwareSerial.h>

// Servo myservo;
const int ENABLE_TX_PIN = 18;
const int ENABLE_RX_PIN = 5;
const int LED_PIN = 2;

unsigned long last_millis = 0;

char buffer[10];
// HardwareSerial serial = HardwareSerial(2);
void setup() {
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  Serial.println("Starting");
  pinMode(ENABLE_TX_PIN, OUTPUT);
  pinMode(ENABLE_RX_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(ENABLE_TX_PIN, HIGH);   // We are the receiver, so we set TX to HIGH
  digitalWrite(ENABLE_RX_PIN, LOW);  // We are the receiver, so we set RX to LOW
  last_millis = millis();
}

void loop() {
  if (millis() - last_millis > 20) {

    int available = Serial2.available();
    if (available) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("R");
      int angle_num_len = Serial2.readBytesUntil('|', buffer, 5);
      // String message = Serial2.readString();
      // Serial.println(message);
      Serial.println(String(buffer).substring(0, angle_num_len));
      digitalWrite(LED_PIN, LOW);
    }
    last_millis = millis();
  }
}