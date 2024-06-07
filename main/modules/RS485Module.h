#define RS485_SET_TX_PIN 22
#define RS485_SET_RX_PIN 22

class RS485Module {
  char serial_buffer[30];

  String command_queue = "";
  public:
    RS485Module() {
      // Serial.begin(115200, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
      pinMode(RS485_SET_TX_PIN, OUTPUT);
      pinMode(RS485_SET_RX_PIN, OUTPUT);
      setToReceiveMode();
    }

    void setToTransmitMode() {
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transamitter
      digitalWrite(RS485_SET_RX_PIN, HIGH); // We set ourselves as the transamitter
    }

    void setToReceiveMode() {
      digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver
      digitalWrite(RS485_SET_RX_PIN, LOW); // We set ourselves as the receiver
    }

    OBECStatus check_for_status_message() {
      if (Serial.available()) {
          digitalWrite(LED_PIN, HIGH);
          delay(800);
          digitalWrite(LED_PIN, LOW);
          int message_len = Serial.readBytesUntil('|', serial_buffer, 30);
          String message = String(serial_buffer).substring(0, message_len);
          Logger::log(String("Received status from OBEC:" + message));
          // Logger::log(String("Length " + String(message_len)));
          if (message_len == 18) { // We only want to parse the message if it was a complete system status message
            if (command_queue.length() != 0)
              delay(5);
              send_valve_command_from_queue();
            return OBECStatus::from_message(message);
          }
      }
      return OBECStatus(-1, -1, -1, -1, false, false);
    }

    void send_valve_command_from_queue() {
      setToTransmitMode();
      Serial.print(command_queue);
      Logger::log(String("Sending valve command to  OBEC:" + command_queue));
      command_queue = "";
      delay(5);
      setToReceiveMode();
    }

    void add_valve_command_to_send_queue(Command command) {
      String command_msg = Command::to_message(command);
      command_queue = command_msg;
    }
};

