#ifndef TORETTOSIMULATIONLIB_H
#define TORETTOSIMULATIONLIB_H
#include "TorettoSimulationLib.h"
#endif // TORETTOSIMULATIONLIB_H

typedef struct TimedSimulationCommand {
  unsigned long command_time_ms;
  SimulationCommand command;
};

TimedSimulationCommand NOMINAL_SCENARIO[] = {
  {1000, SimulationCommand(SENSOR_MODULE, 0, "14")} // get_loading_line_pressure_psi 14
};
