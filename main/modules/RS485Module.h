#define RS485_RX_PIN 16
#define RS485_TX_PIN 17
#define RS485_SET_TX_PIN 22
#define RS485_SET_RX_PIN 22

class RS485Module {
  char serial_buffer[30];

  public:
    RS485Module() {
      // Serial.begin(115200, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
      pinMode(RS485_SET_TX_PIN, OUTPUT);
      pinMode(RS485_SET_RX_PIN, OUTPUT);
      setToReceiveMode();
    }

    void setToReceiveMode() {
      digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver
      digitalWrite(RS485_SET_RX_PIN, LOW); // We set ourselves as the receiver
    }

    OBECStatus check_for_status_message() {
      if (Serial.available()) {
        // Logger::blink_debug_led_times(2);
        int message_len = Serial.readBytesUntil('|', serial_buffer, 30);
        String message = String(serial_buffer).substring(0, message_len);
        // Logger::log(String("Received status from OBEC:" + message));
        Serial.println(message);
        if (message_len >= 10) { // We only want to parse the message if it was a complete system status message
          // Logger::blink_debug_led_times(4);
          return OBECStatus::from_message(message);
        }
      }
      return OBECStatus(-1, -1);
    }
};

