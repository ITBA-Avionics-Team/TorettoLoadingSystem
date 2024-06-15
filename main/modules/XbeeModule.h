#include <stdlib.h>
#include <stdint.h>

class XBeeModule {
  char serial_buffer[30];
  public:

    XBeeModule() {
      pinMode(5, OUTPUT);
      digitalWrite(5, HIGH); 
    }

    Command check_for_commands(){ 
      if (Serial2.available()) {
            int message_len = Serial2.readBytesUntil('|', serial_buffer, 30);
            String message = String(serial_buffer).substring(0, message_len);
            return Command::from_message(message);
      }
      return Command(EMPTY);
    }

    void send_system_status(SystemStatus system_status){
      String system_status_msg = SystemStatus::to_message(system_status) + String("|");
      Serial2.println(system_status_msg);
      Logger::log(String("Sending system status message to MCC ") + system_status_msg);
    }

    void send_flight_computer_status(FlightComputersStatus data) {

    }

    void send_tank_depress_vent_temp_low() {
      Serial2.print("TDVTL|");
    }

    void send_wind_abort() {
      Serial2.print("WABRT|");
    }

    void send_umbrilical_abort() {
      Serial2.print("UABRT|");
    }

    void send_external_vent_as_default_prompt(){
      Serial2.print("EXTVD|");
    }

    void send_ignition_confirmation() {
      Serial2.print("IGNCO|");
    }
};
