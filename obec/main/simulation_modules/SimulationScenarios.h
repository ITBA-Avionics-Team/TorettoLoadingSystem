#ifndef TORETTOSIMULATIONLIB_H
#define TORETTOSIMULATIONLIB_H
#include "TorettoSimulationLib.h"
#endif // TORETTOSIMULATIONLIB_H

typedef struct TimedSimulationCommand {
  unsigned long command_time_ms;
  SimulationCommand command;
};

TimedSimulationCommand NOMINAL_SCENARIO[] = {
  {1000, SimulationCommand(SENSOR_MODULE, 0, "60")}, // get_tank_pressure_psi 60
  {1000, SimulationCommand(SENSOR_MODULE, 1, "20")}, // get_tank_temperature_celsius 20
  {1000, SimulationCommand(SENSOR_MODULE, 2, "24")}, // get_tank_depres_vent_temperature_celsius 24
  {1000, SimulationCommand(SENSOR_MODULE, 3, "4.17")}, // get_obec_battery_voltage_volt 4.17
  {1000, SimulationCommand(SENSOR_MODULE, 4, "0")}, // get_tank_depres_vent_valve_open 0
  {1000, SimulationCommand(SENSOR_MODULE, 5, "0")}, // get_engine_valve_open 0

  {4000, SimulationCommand(SENSOR_MODULE, 0, "98")}, // get_loading_line_pressure_psi 98
  {7000, SimulationCommand(SENSOR_MODULE, 0, "110")}, // get_loading_line_pressure_psi 110
};
