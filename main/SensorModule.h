
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

class SensorModule {
  public:

  void init() {
    Logger::log("Initializing sensor Module...");

  
    Logger::log("Sensor Module initialized.");
  }

  int get_loading_line_pressure_psi(){
    return 0;
  }

  float get_lc_battery_voltage_volt(){
    return 0;
  }

  bool get_obec_connection_ok(){
    return true; //TODO: Implement
  }

  bool get_loading_valve_open(){
    return true; //TODO: Implement
  }

  bool get_loading_depress_vent_valve_open(){
    return true; //TODO: Implement
  }

  bool get_hydraulic_umbrilical_connected() {
    return true; //TODO: Implement
  }

  bool get_igniter_continuity_ok(){
    return true; //TODO: Implement
  }

};
