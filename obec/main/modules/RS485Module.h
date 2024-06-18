#define RS485_RX_PIN 16
#define RS485_TX_PIN 17
#define RS485_SET_TX_PIN 5
#define RS485_SET_RX_PIN 18

class RS485Module {
  char serial_buffer[30];
  public:
    RS485Module() {
      
      // setToTransmitMode();
    }

    void setToTransmitMode() {
      digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transamitter
      digitalWrite(RS485_SET_RX_PIN, HIGH); // We set ourselves as the transamitter
      // Serial.println("Set to transmitter");
    }

    void send_system_status(OBECStatus system_status) {
      // setToTransmitMode();
      String system_status_msg = OBECStatus::to_message(system_status) + String("|");
      Serial2.print(system_status_msg);
      // Serial.print(system_status_msg);
      Logger::log("Sending" + system_status_msg);
    }
};
