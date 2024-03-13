#ifndef TORETTOSIMULATIONLIB_H
#define TORETTOSIMULATIONLIB_H
#include "TorettoSimulationLib.h"
#endif // TORETTOSIMULATIONLIB_H

typedef struct TimedSimulationCommand {
  unsigned long command_time_ms;
  SimulationCommand command;
};

TimedSimulationCommand NOMINAL_SCENARIO[] = {
  {1000, SimulationCommand(SENSOR_MODULE, 0, "14")}, // get_loading_line_pressure_psi 14
  {1000, SimulationCommand(SENSOR_MODULE, 1, "4.2")}, // get_lc_battery_voltage_volt 4.2
  {1000, SimulationCommand(SENSOR_MODULE, 2, "1")}, // get_obec_connection_ok 1
  {1000, SimulationCommand(SENSOR_MODULE, 3, "0")}, // get_loading_valve_open 0
  {1000, SimulationCommand(SENSOR_MODULE, 4, "0")}, // get_loading_depress_vent_valve_open 0
  {1000, SimulationCommand(SENSOR_MODULE, 5, "1")}, // get_hydraulic_umbrilical_connected 1
  {1000, SimulationCommand(SENSOR_MODULE, 6, "1")}, // get_igniter_continuity_ok 1

  {4000, SimulationCommand(SENSOR_MODULE, 0, "98")}, // get_loading_line_pressure_psi 98
  {7000, SimulationCommand(SENSOR_MODULE, 0, "110")}, // get_loading_line_pressure_psi 110

  {7000, SimulationCommand(SENSOR_MODULE, 3, "1")}, // get_loading_valve_open 1
  {8000, SimulationCommand(SENSOR_MODULE, 3, "0")}, // get_loading_valve_open 0
  {9000, SimulationCommand(SENSOR_MODULE, 3, "1")}, // get_loading_valve_open 1
  {10000, SimulationCommand(SENSOR_MODULE, 3, "0")}, // get_loading_valve_open 0
};
