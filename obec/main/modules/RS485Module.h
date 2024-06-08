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
      setToTransmitMode();
    }

    void setToTransmitMode() {
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transamitter
      digitalWrite(RS485_SET_RX_PIN, HIGH); // We set ourselves as the transamitter
    }

    void setToReceiveMode() {
      digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver
      digitalWrite(RS485_SET_RX_PIN, LOW); // We set ourselves as the receiver
    }

    Command check_for_commands() {
      // digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      Logger::log("Checking for commands...");
      if (Serial2.available()) {
        Logger::blink_debug_led_times(1);
        // Logger::log("Received command.");
        int message_len = Serial2.readBytesUntil('|', serial_buffer, 30);
        String message = String(serial_buffer).substring(0, message_len);
        Logger::log("Received command: " + message);
        Logger::log("Length: " + String(message_len));
        if (message_len == 7) // We only want to parse the message if it was a complete system status message
          return Command::from_message(message);
      }
      return Command(EMPTY);
    }

    void send_system_status(OBECStatus system_status) {
      setToTransmitMode();
      String system_status_msg = OBECStatus::to_message(system_status) + String("|");
      Serial2.print(system_status_msg);
      delay(5);
      setToReceiveMode();
      // Serial.print(system_status_msg);
      Logger::log("Sending" + system_status_msg);
    }
};
