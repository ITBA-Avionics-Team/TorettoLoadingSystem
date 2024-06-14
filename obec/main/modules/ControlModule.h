#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "../TorettoLib.h"
#endif // TORETTOLIB_H

#define ENGINE_VALVE_PIN 23 // Relay (High/Low)

class ControlModule
{

  SimulatedValveStatus &simulated_valve_status;

public:

  ControlModule(SimulatedValveStatus& simulated_valve_status_val) : simulated_valve_status(simulated_valve_status_val) {}

  void init()
  {
    Logger::log("Initializing Control Module...");
    pinMode(ENGINE_VALVE_PIN, OUTPUT);
    Logger::log("Control Module initialized.");
  }

  void execute_valve_command(Command command)
  {
    switch (command.valve)
    {
    case ENGINE_VALVE:
      move_relay(ENGINE_VALVE_PIN, command.uint_value);
      simulated_valve_status.engine_valve_open = value > 127.5;
      break;
    }
  }

  void move_relay(uint8_t pin, uint8_t value) {
    digitalWrite(pin, value > 127.5 ? HIGH : LOW);
  }
};
