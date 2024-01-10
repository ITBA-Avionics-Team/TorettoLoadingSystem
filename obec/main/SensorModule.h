
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

class SensorModule
{
public:
  void init()
  {
    Logger::log("Initializing sensor Module...");

    Logger::log("Sensor Module initialized.");
  }

  int get_tank_pressure_psi()
  {
    return 0;
  }

  int get_tank_temperature_celsius()
  {
    return 0;
  }

  float get_tank_depres_vent_temperature_celsius()
  {
    return 0;
  }

  float get_obec_battery_voltage_volt()
  {
    return 0;
  }

  bool get_tank_depres_vent_valve_open()
  {
    return true; // TODO: Implement
  }

  bool get_engine_valve_open()
  {
    return true; // TODO: Implement
  }
};
