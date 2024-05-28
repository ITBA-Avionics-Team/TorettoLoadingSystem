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
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      digitalWrite(RS485_SET_RX_PIN, LOW); // We set ourselves as the transmitter 
    }

    Command check_for_commands() {
      // digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      // if (Serial2.available()) {
      //       int message_len = Serial.readBytesUntil('|', serial_buffer, 30);
      //       String message = String(serial_buffer).substring(0, message_len);
      //       return Command::from_message(message);
      // }
      // return Command(EMPTY);
    }

    void send_system_status(OBECStatus system_status) {
      // digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      String system_status_msg = OBECStatus::to_message(system_status) + String("|");
      Serial2.print(system_status_msg);
      // Serial.print(system_status_msg);
      Logger::log("Sending" + system_status_msg);
    }
};
