#include <stdlib.h>
#include <stdint.h>
#ifndef TORETTOLIB_H
#define TORETTOLIB_H
#include "TorettoLib.h"
#endif // TORETTOLIB_H
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

class SimulatedSensorModule {

  SimulationModule& simulation_module;
  public:

  SimulatedSensorModule(SimulationModule& simulation_module): simulation_module(simulation_module){};

  void init() {
    Logger::log("Initializing simulated sensor Module...");

  
    Logger::log("Simulated Sensor Module initialized.");
  }

  int get_loading_line_pressure_psi(){
    return SimulationModule::parse_int_str(simulation_module.sensor_module_return_vals[0]);
  }

  float get_lc_battery_voltage_volt(){
    return SimulationModule::parse_float_str(simulation_module.sensor_module_return_vals[1]);
  }

  bool get_obec_connection_ok(){
    return SimulationModule::parse_bool_str(simulation_module.sensor_module_return_vals[2]);
  }

  bool get_loading_valve_open(){
    return SimulationModule::parse_bool_str(simulation_module.sensor_module_return_vals[3]);
  }

  bool get_loading_depress_vent_valve_open(){
    return SimulationModule::parse_bool_str(simulation_module.sensor_module_return_vals[4]);
  }

  bool get_hydraulic_umbrilical_connected() {
    return SimulationModule::parse_bool_str(simulation_module.sensor_module_return_vals[5]);
  }

  bool get_igniter_continuity_ok(){
    return SimulationModule::parse_bool_str(simulation_module.sensor_module_return_vals[6]);
  }

};
