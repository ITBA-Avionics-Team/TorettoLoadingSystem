#define RS485_SET_TX_PIN 18
#define RS485_SET_RX_PIN 5

class RS485Module {
  char serial_buffer[30];
  public:
    RS485Module() {
      Serial2.begin(115200, SERIAL_8N1, 16, 17);
      pinMode(RS485_SET_TX_PIN, OUTPUT);
      pinMode(RS485_SET_RX_PIN, OUTPUT);
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We are the receiver, so we set TX to HIGH
      digitalWrite(RS485_SET_RX_PIN, LOW); // We are the receiver, so we set RX to LOW
    }

    OBECStatus check_for_status_message() {
      // digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      if (Serial2.available()) {
          int message_len = Serial2.readBytesUntil('|', serial_buffer, 30);
          String message = String(serial_buffer).substring(0, message_len);
          Logger::debug(String("Received status from OBEC:" + message);
          return OBECStatus::from_message(message);
      }
      // digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      return OBECStatus(-1, -1, -1, -1, false, false);
    }

    void send_valve_command(Command command) {
      // digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      String command_msg = Command::to_message(command) + String("|");
      Serial2.print(command_msg);
      // digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
    }
};

