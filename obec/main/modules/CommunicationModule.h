#include <stdlib.h>
#include <stdint.h>
#ifndef TORETTOLIB_H
#define TORETTOLIB_H
#include "../TorettoLib.h"
#endif // TORETTOLIB_H

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
  RS485Module lc_communication;

  public:
    Command latest_LC_command;

    bool new_LC_command_available = false;

    CommunicationModule() {
      // latest_MCC_command = Command(SetExternalVentAsDefaultCommand, false);
    }

    void init() {
      Logger::log("Initializing Communication Module...");

  
      Logger::log("Communication Module initialized.");
    }

    void send_system_status_to_LC(OBECStatus system_status) {
      lc_communication.send_system_status(system_status);
    }

    void check_for_LC_commands(){
      Command command = lc_communication.check_for_commands();
      if (!Command::is_empty(command)) {
        latest_LC_command = command;
        new_LC_command_available = true;
      }
    }
};
