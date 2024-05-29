#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "../TorettoLib.h"
#endif // TORETTOLIB_H

#include <ESP32Servo.h>

#define TANK_DEPRESS_VENT_VALVE_SERVO_PIN 23 //TODO: DEFINE ACTUAL PIN
#define ENGINE_VALVE_SERVO_PIN 23 //TODO: DEFINE ACTUAL PIN

class ControlModule
{

  SimulatedValveStatus &simulated_valve_status;

public:

  Servo tankDepressVentValveServo;
  Servo engineValveServo;

  ControlModule(SimulatedValveStatus& simulated_valve_status_val) : simulated_valve_status(simulated_valve_status_val) {}

  void init()
  {
    Logger::log("Initializing Control Module...");
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    tankDepressVentValveServo.setPeriodHertz(50);    // standard 50 hz servo
    tankDepressVentValveServo.attach(TANK_DEPRESS_VENT_VALVE_SERVO_PIN, 1000, 2000); // attaches the servo on pin 18 to the servo object
    tankDepressVentValveServo.write(90);

    engineValveServo.setPeriodHertz(50);    // standard 50 hz servo
    engineValveServo.attach(ENGINE_VALVE_SERVO_PIN, 1000, 2000); // attaches the servo on pin 18 to the servo object
    engineValveServo.write(90);

    Logger::log("Control Module initialized.");
  }

  void execute_valve_command(Command command)
  {
    switch (command.valve)
    {
    case TANK_DEPRESS_VENT_VALVE:
      move_tank_depress_vent_valve_servo(command.uint_value);
      break;
    case ENGINE_VALVE:
      move_engine_valve_servo(command.uint_value);
      break;
    }
  }

  void move_tank_depress_vent_valve_servo(uint8_t value) {
    tankDepressVentValveServo.write(180 * (value / 255));
    simulated_valve_status.tank_depress_vent_valve_open = value > 127.5;
  }

  void move_engine_valve_servo(uint8_t value) {
    engineValveServo.write(180 * (value / 255));
    simulated_valve_status.engine_valve_open = value > 127.5;
  }
};
