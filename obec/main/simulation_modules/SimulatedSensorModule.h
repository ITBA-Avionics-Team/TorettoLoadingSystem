#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

class SimulatedSensorModule
{

  SimulationModule& simulation_module;
public:

  SimulatedSensorModule(SimulationModule& simulation_module): simulation_module(simulation_module){};

  void init()
  {
    Logger::log("Initializing simulated sensor Module...");

    Logger::log("Simulated Sensor Module initialized.");
  }

  int get_tank_pressure_bar()
  {
    return SimulationModule::parse_int_str(simulation_module.sensor_module_return_vals[0]);
  }

  int get_tank_temperature_celsius()
  {
    return SimulationModule::parse_int_str(simulation_module.sensor_module_return_vals[1]);
  }

  float get_tank_depres_vent_temperature_celsius()
  {
    return SimulationModule::parse_float_str(simulation_module.sensor_module_return_vals[2]);
  }

  float get_obec_battery_voltage_volt()
  {
    return SimulationModule::parse_float_str(simulation_module.sensor_module_return_vals[3]);
  }

  bool get_tank_depres_vent_valve_open()
  {
    return SimulationModule::parse_bool_str(simulation_module.sensor_module_return_vals[4]);
  }

  bool get_engine_valve_open()
  {
    return SimulationModule::parse_bool_str(simulation_module.sensor_module_return_vals[5]);
  }
};
