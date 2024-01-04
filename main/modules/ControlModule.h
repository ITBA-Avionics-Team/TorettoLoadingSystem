#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "TorettoLib.h"
#endif // TORETTOLIB_H

class ControlModule {

  public:

    void execute_valve_command(Command command) {
      Logger::debug("ControlModule.execute_valve_command" + String(command.valve));
    }

    void set_obec_power(bool on) {
      Logger::debug("Control.set_obec_power" + on);
    }

    void set_igniters_on(bool value) {
      Logger::debug("Control.set_igniters_on" + value);
    }
    
    void disconnect_umbrilical() {
      Logger::debug("Control.disconnect_umbrilical");
    }

};
