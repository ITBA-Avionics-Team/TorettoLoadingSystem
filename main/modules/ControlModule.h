#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "TorettoLib.h"
#endif // TORETTOLIB_H

#include <ESP32Servo.h>

#define LOADING_VALVE_PIN 14 // Relé (high low)
#define LOADING_LINE_DEPRESS_VENT_VALVE_PIN 13 // Servo
#define NOX_UMBRILICAL_PIN 27 // Relé (high low)
#define IGNITERS_PIN 33 // Relé (high low)

class ControlModule {
  

  public:

    Servo loadingValveServo;

    void init() {
      // Allow allocation of all timers
      ESP32PWM::allocateTimer(0);
      ESP32PWM::allocateTimer(1);
      ESP32PWM::allocateTimer(2);
      ESP32PWM::allocateTimer(3);
      loadingValveServo.setPeriodHertz(50);    // standard 50 hz servo
      loadingValveServo.attach(LOADING_LINE_DEPRESS_VENT_VALVE_PIN, 1000, 2000); // attaches the servo on pin 18 to the servo object
      loadingValveServo.write(60);
      delay(500);
      loadingValveServo.write(180);

      pinMode(LOADING_VALVE_PIN, OUTPUT);
      pinMode(NOX_UMBRILICAL_PIN, OUTPUT);
      pinMode(IGNITERS_PIN, OUTPUT);
      delay(500);
      
      move_loading_valve_relay(Open);
      delay(500);
      move_loading_valve_relay(Close);


    }

    void execute_valve_command(Command command) {
      // Logger::debug("ControlModule.execute_valve_command" + String(command.valve));
      // move_loading_valve_servo(command.uint_value);
      switch (command.valve) {
        case LOADING_VALVE:
          move_loading_valve_relay(command.uint_value);
          break;
        case LOADING_LINE_DEPRESS_VENT_VALVE:
          move_loading_valve_servo(command.uint_value);
          break;
        default:
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
      move_umbrilical_relay(0); // 0 is disconnected, 255 is connected
    }

    void move_loading_valve_relay(uint8_t value) {
      digitalWrite(LOADING_VALVE_PIN, value > 127.5 ? HIGH : LOW);
    }

    void move_umbrilical_relay(uint8_t value) {
      digitalWrite(NOX_UMBRILICAL_PIN, value > 127.5 ? HIGH : LOW);
    }

    void move_loading_valve_servo(uint8_t value) {
      loadingValveServo.write(180);
    }

};
