#include <stdlib.h>
#include <stdint.h>
#ifndef TORETTOLIB_H
#define TORETTOLIB_H
#include "TorettoLib.h"
#endif // TORETTOLIB_H

#include "modules/XbeeModule.h"
#include "modules/RS485Module.h"

#define STATE_IDLE 0
#define STATE_WAITING_FOR_GROUND_RESPONSE 1
#define STATE_WAITING_FOR_PAYLOAD_1_RESPONSE 2
#define STATE_WAITING_FOR_PAYLOAD_2_RESPONSE 3

#define SIMULATION_DISABLED 0
#define SIMULATION_ENABLED 1
#define SIMULATION_ACTIVATED 2

#define PAYLOAD_MAX_PACKAGE_LENGTH 42

#define PACKET_COUNT_EEPROM_ADDR 4

class CommunicationModule {
  XBeeModule mcc_xbee;
  RS485Module obec_communication;

  Command latest_MCC_command;
  OBECStatus latest_OBEC_status;

  bool new_MCC_command_available = false;
  bool new_OBEC_status_available = false;

  public:

    CommunicationModule() {
      // latest_MCC_command = Command(SetExternalVentAsDefaultCommand, false);
    }

    Command get_latest_MCC_command() {

    }

    OBECStatus get_latest_OBEC_status() {
      
    }

    bool get_new_MCC_command_available() {
      return new_MCC_command_available;
    }

    void set_new_MCC_command_available(bool val) {
      new_MCC_command_available = val;
    }

    bool get_new_OBEC_status_available() {
      return new_OBEC_status_available;
    }

    void set_new_OBEC_status_available(bool val) {
      new_OBEC_status_available = val;
    }

    void send_valve_command_to_OBEC(Command command) {
      obec_communication.send_valve_command(command);
    }

    void send_abort_signal_to_OBEC() {
      Logger::debug("Communication.send_abort_signal_to_OBEC");
    }

    void send_ignition_confirmation_to_OBEC() {
      Logger::debug("Communication.send_ignition_confirmation_to_OBEC");
    }

    void send_system_status_to_MCC(SystemStatus system_status) {
      Logger::debug("Communication.send_system_status_to_MCC");
      mcc_xbee.send_system_status(system_status);
    }

    void send_preflight_check_data_to_MCC(PreflightCheckData data) {
      Logger::debug("Communication.send_preflight_check_data_to_MCC");
      mcc_xbee.send_preflight_check(data);
    }

    void send_stale_flight_check_to_MCC() {
      Logger::debug("Communication.send_stale_flight_check_to_MCC");
    }

    void send_wind_abort_to_MCC() {
      Logger::debug("Communication.send_wind_abort_to_MCC");
      mcc_xbee.send_wind_abort();
    }

    void send_umbrilical_abort_to_MCC() {
      Logger::debug("Communication.send_umbrilical_abort_to_MCC");
      mcc_xbee.send_umbrilical_abort();
    }

    void send_tank_depress_vent_temp_low_to_MCC() {
      Logger::debug("Communication.send_tank_depress_vent_temp_low_to_MCC");
      mcc_xbee.send_tank_depress_vent_temp_low();
    }

    void send_external_vent_as_default_prompt_to_MCC() {
      Logger::debug("Communication.send_external_vent_as_default_prompt_to_MCC");
      mcc_xbee.send_external_vent_as_default_prompt();
    }

    void send_ignition_confirmation_to_MCC() {
      Logger::debug("Communication.send_ignition_confirmation_to_MCC");
      mcc_xbee.send_ignition_confirmation();
    }

    void check_for_MCC_commands(){
      Command command = mcc_xbee.check_for_commands();
      if (!Command::is_empty(command)) {
        latest_MCC_command = command;
        new_MCC_command_available = true;
      }
    }

    void check_for_OBEC_status(){
      OBECStatus status = obec_communication.check_for_status_message();
      if (!OBECStatus::is_empty(status)) {
        latest_OBEC_status = status;
        new_OBEC_status_available = true;
      }
    }
};
