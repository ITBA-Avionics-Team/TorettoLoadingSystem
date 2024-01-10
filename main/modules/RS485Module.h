#define RS485_SET_TX_PIN 2

class RS485Module {
  HardwareSerial serial = HardwareSerial(2);;
  char serial_buffer[30];
  public:
    RS485Module() {
      serial.begin(9600, SERIAL_8N1, 13, -1);
      pinMode(RS485_SET_TX_PIN, OUTPUT);
    }

    OBECStatus check_for_status_message() {
      digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      if (serial.available()) {
            int message_len = serial.readBytesUntil('|', serial_buffer, 30);
            String message = String(serial_buffer).substring(0, message_len);
            return OBECStatus::from_message(message);
      }
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      return OBECStatus(-1, -1, -1, -1, false, false);
    }

    void send_valve_command(Command command) {
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      String command_msg = Command::to_message(command) + String("|");
      serial.print(command_msg);
      digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
    }
};

