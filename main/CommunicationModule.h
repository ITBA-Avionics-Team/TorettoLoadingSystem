#include <stdlib.h>
#include <stdint.h>
#ifndef TORETTOLIB_H
#define TORETTOLIB_H
#include "TorettoLib.h"
#endif // TORETTOLIB_H

#include "XbeeModule.h"
#include "RS485Module.h"

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

  public:
    void send_valve_command_to_OBEC(ValveCommand command) {\
      obec_communication.send_valve_command(command);
    }

    void send_abort_signal_to_OBEC() {

    }

    void send_ignition_confirmation_to_OBEC() {

    }

    void send_system_status_to_MCC(SystemStatus system_status) {
      mcc_xbee.send_system_status(system_status);
    }

    void send_preflight_check_data_to_MCC(PreflightCheckData data) {
      mcc_xbee.send_preflight_check(data);
    }

    void send_stale_flight_check_to_MCC() {

    }

    void send_wind_abort_to_MCC() {

    }

    void send_umbrilical_abort_to_MCC() {

    }

    void send_tank_depress_vent_temp_low_to_MCC() {
      mcc_xbee.send_tank_depress_vent_tamp_low();
    }

    void send_external_vent_as_default_prompt_to_MCC() {
      mcc_xbee.send_external_vent_as_default_prompt();
    }

    void send_ignition_confirmation_to_MCC() {
      mcc_xbee.send_ignition_confirmation();
    }

    ValveCommand check_for_MCC_commands(){
      ValveCommand command = mcc_xbee.check_for_commands();
      return command;
    }

    OBECStatus check_for_OBEC_status(){
      OBECStatus status = obec_communication.check_for_status_message();
      return status;
    }
};
