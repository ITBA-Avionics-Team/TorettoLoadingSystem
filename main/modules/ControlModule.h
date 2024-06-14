#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "TorettoLib.h"
#endif // TORETTOLIB_H

#include <ESP32Servo.h>

#define LOADING_VALVE_PIN 14 // Relé (high low)
#define LOADING_LINE_DEPRESS_VENT_VALVE_PIN 19 // Relé (high low)
#define NOX_UMBRILICAL_PIN 27 // Relé (high low)
#define IGNITERS_PIN 13 // Relé (high low)

class ControlModule {

  SimulatedValveStatus& simulated_valve_status;
  
  public:

    Servo loadingValveServo;

    ControlModule(SimulatedValveStatus& simulated_valve_status_val) : simulated_valve_status(simulated_valve_status_val) {}

    void init() {
      // Allow allocation of all timers
        
      delay(500);

      pinMode(LOADING_VALVE_PIN, OUTPUT);
      pinMode(LOADING_LINE_DEPRESS_VENT_VALVE_PIN, OUTPUT);
      pinMode(NOX_UMBRILICAL_PIN, OUTPUT);
      pinMode(IGNITERS_PIN, OUTPUT);
      delay(1000);
      
      move_relay(LOADING_VALVE_PIN, Open);
      delay(500);
      move_relay(LOADING_VALVE_PIN, Close);
      delay(1000);
      move_relay(LOADING_LINE_DEPRESS_VENT_VALVE_PIN, Open);
      delay(500);
      move_relay(LOADING_LINE_DEPRESS_VENT_VALVE_PIN, Close);
      delay(1000);
      move_relay(NOX_UMBRILICAL_PIN, Open);
      delay(500);
      move_relay(NOX_UMBRILICAL_PIN, Close);
      delay(1000);
      move_relay(IGNITERS_PIN, Open);
      delay(500);
      move_relay(IGNITERS_PIN, Close);

    }

    void execute_valve_command(Command command) {
      char uint_string[4];
      sprintf(uint_string, "%d", command.uint_value);
      Logger::debug("ControlModule.execute_valve_command: " + String(command.valve) + " " + String(uint_string));
      // move_loading_valve_servo(command.uint_value);
      switch (command.valve) {
        case LOADING_VALVE:
          move_relay(LOADING_VALVE_PIN, command.uint_value);
          simulated_valve_status.loading_valve_open = command.uint_value > 127.5;
          break;
        case LOADING_LINE_DEPRESS_VENT_VALVE:
          move_relay(LOADING_LINE_DEPRESS_VENT_VALVE_PIN, command.uint_value);
          simulated_valve_status.loading_depress_vent_valve_open = command.uint_value > 127.5;
          break;
      }
    }

    //Deprecated
    void set_obec_power(bool on) {
      Logger::debug("[DEPRECATED] Control.set_obec_power" + on);
    }

    void set_igniters_on(bool value) {
      Logger::debug("Control.set_igniters_on" + value);
    }
    
    void disconnect_umbrilical() {
      Logger::debug("Control.disconnect_umbrilical");
      move_relay(NOX_UMBRILICAL_PIN, 0); // 0 is disconnected, 255 is connected
    }

    void move_relay(uint8_t pin, uint8_t value) {
      digitalWrite(pin, value > 127.5 ? HIGH : LOW);
    }

    void move_umbrilical_relay(uint8_t value) {
      digitalWrite(NOX_UMBRILICAL_PIN, value > 127.5 ? HIGH : LOW);
    }

};
