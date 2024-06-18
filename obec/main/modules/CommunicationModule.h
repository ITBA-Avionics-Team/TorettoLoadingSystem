#include <stdlib.h>
#include <stdint.h>
#ifndef TORETTOLIB_H
#define TORETTOLIB_H
#include "../TorettoLib.h"
#endif // TORETTOLIB_H

#include "RS485Module.h"

class CommunicationModule {
  RS485Module lc_communication;

  public:

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
};
