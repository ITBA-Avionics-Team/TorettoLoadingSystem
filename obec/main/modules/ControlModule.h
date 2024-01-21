#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "../TorettoLib.h"
#endif // TORETTOLIB_H

class ControlModule {

  public:

  void init() {
    Logger::log("Initializing Control Module...");

  
    Logger::log("Sensor Module initialized.");
  }

  void execute_valve_command(Command command) {

  }
};
