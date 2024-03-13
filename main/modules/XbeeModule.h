#include "Queue.h"
#include <stdlib.h>
#include <stdint.h>
#include <XBee.h>

#define STATE_IDLE 0
#define STATE_WAITING_FOR_GROUND_RESPONSE 1
#define STATE_WAITING_FOR_PAYLOAD_1_RESPONSE 2
#define STATE_WAITING_FOR_PAYLOAD_2_RESPONSE 3

#define SIMULATION_DISABLED 0
#define SIMULATION_ENABLED 1
#define SIMULATION_ACTIVATED 2

#define PAYLOAD_MAX_PACKAGE_LENGTH 42

#define PACKET_COUNT_EEPROM_ADDR 4

class XBeeModule {
  char serial_buffer[30];
  public:

    Command check_for_commands(){
      // digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      if (Serial.available()) {
            int message_len = Serial.readBytesUntil('|', serial_buffer, 30);
            String message = String(serial_buffer).substring(0, message_len);
            return Command::from_message(message);
      }
      return Command(EMPTY);
    }

    void send_system_status(SystemStatus system_status){
      // digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      
      String system_status_msg = SystemStatus::to_message(system_status) + String("|");
      Logger::log(String("Sending system status message to MCC ") + system_status_msg);
    }

    void send_preflight_check(PreflightCheckData data) {
    }

    void send_tank_depress_vent_temp_low() {
      Serial.print("TDVTL|");
    }

    void send_wind_abort() {
      Serial.print("WABRT|");
    }

    void send_umbrilical_abort() {
      Serial.print("UABRT|");
    }

    void send_external_vent_as_default_prompt(){
      Serial.print("EXTVD|");
    }

    void send_ignition_confirmation() {
      Serial.print("IGNCO|");
    }
};
