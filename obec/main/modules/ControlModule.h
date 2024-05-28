#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "../TorettoLib.h"
#endif // TORETTOLIB_H

class ControlModule
{

  SimulatedValveStatus &simulated_valve_status;

public:
  ControlModule(SimulatedValveStatus& simulated_valve_status_val) : simulated_valve_status(simulated_valve_status_val) {}

  void init()
  {
    Logger::log("Initializing Control Module...");
    Logger::log("Sensor Module initialized.");
  }

  void execute_valve_command(Command command)
  {
    switch (command.valve)
    {
    case TANK_DEPRESS_VENT_VALVE:
      // move_tank_depress_vent_valve_relay(command.uint_value);
      break;
    case ENGINE_VALVE:
      // move_engine_valve_servo(command.uint_value);
      break;
    }
  }
};
