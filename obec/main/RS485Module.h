#define RS485_SET_TX_PIN 2;

class RS485Module {
  HardwareSerial serial = HardwareSerial(2);;
  char serial_buffer[30];
  public:

    RS485Module() {
      serial.begin(9600, SERIAL_8N1, 13, -1);
      pinMode(RS485_SET_TX_PIN, OUTPUT);
    }

    Command check_for_commands() {
      digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      if (serial.available()) {
            int message_len = serial.readBytesUntil('|', serial_buffer, 30);
            String message = String(serial_buffer).substring(0, message_len);
            return Command::from_message(message);
      }
      return Command(EMPTY);
    }

    void send_system_status(OBECStatus system_status) {
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      String system_status_msg = OBECStatus::to_message(system_status);
      serial.print(system_status_msg);
    }
};
