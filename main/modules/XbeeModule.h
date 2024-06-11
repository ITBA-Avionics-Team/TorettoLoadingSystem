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

#define RS485_RX_PIN 16
#define RS485_TX_PIN 17


class XBeeModule {
  char serial_buffer[30];
  public:

    XBeeModule() {
      pinMode(5, OUTPUT);
      digitalWrite(5, HIGH); 
    }

    Command check_for_commands(){
      // digitalWrite(RS485_SET_TX_PIN, LOW); // We set ourselves as the receiver 
      if (Serial2.available()) {
            int message_len = Serial2.readBytesUntil('|', serial_buffer, 30);
            String message = String(serial_buffer).substring(0, message_len);
            return Command::from_message(message);
      }
      return Command(EMPTY);
    }

    void send_system_status(SystemStatus system_status){
      // digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transmitter 
      
      String system_status_msg = SystemStatus::to_message(system_status) + String("|");
      Serial2.println(system_status_msg);
      Logger::log(String("Sending system status message to MCC ") + system_status_msg);
    }

    void send_preflight_check(PreflightCheckData data) {
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
