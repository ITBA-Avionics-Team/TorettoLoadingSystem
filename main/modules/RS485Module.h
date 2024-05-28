#define RS485_RX_PIN 16
#define RS485_TX_PIN 17
#define RS485_SET_TX_PIN 5
#define RS485_SET_RX_PIN 18

class RS485Module {
  char serial_buffer[30];
  public:
    RS485Module() {
      Serial2.begin(115200, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
      pinMode(RS485_SET_TX_PIN, OUTPUT);
      pinMode(RS485_SET_RX_PIN, OUTPUT);
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We are the receiver, so we set TX to HIGH
      digitalWrite(RS485_SET_RX_PIN, LOW); // We are the receiver, so we set RX to LOW
    }

    OBECStatus check_for_status_message() {
      if (Serial2.available()) {
          digitalWrite(LED_PIN, HIGH);
          delay(800);
          digitalWrite(LED_PIN, LOW);
          int message_len = Serial2.readBytesUntil('|', serial_buffer, 30);
          String message = String(serial_buffer).substring(0, message_len);
          Logger::log(String("Received status from OBEC:" + message));
          // Logger::log(String("Length " + String(message_len)));
          if (message_len == 18) // We only want to parse the message if it was a complete system status message
            return OBECStatus::from_message(message);
      }
      return OBECStatus(-1, -1, -1, -1, false, false);
    }

    void send_valve_command(Command command) {
      Logger::log(String("Sending valve command to  OBEC."));
      String command_msg = Command::to_message(command) + String("|");
      Serial2.print(command_msg);
    }
};

