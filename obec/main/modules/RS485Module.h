#define RS485_SET_TX_PIN 2

class RS485Module {
  char serial_buffer[30];
  public:

    RS485Module() {
      pinMode(RS485_SET_TX_PIN, OUTPUT);
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
    }

    Command check_for_commands() {
      // digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      if (Serial.available()) {
            int message_len = Serial.readBytesUntil('|', serial_buffer, 30);
            String message = String(serial_buffer).substring(0, message_len);
            return Command::from_message(message);
      }
      return Command(EMPTY);
    }

    void send_system_status(OBECStatus system_status) {
      // digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      String system_status_msg = OBECStatus::to_message(system_status) + String("|");
      Serial.print(system_status_msg);
    }
};
