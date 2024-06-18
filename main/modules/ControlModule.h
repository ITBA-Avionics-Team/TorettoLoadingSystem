#include <ESP32Servo.h>

#define NOX_UMBRILICAL_PIN 27 // Relé (high low)
#define IGNITERS_PIN 14 // Relé (high low)
#define LOADING_VALVE_PIN 13 // Relé (high low)
#define ENGINE_VALVE_PIN 19 // Relé (high low)

class ControlModule {

  SimulatedValveStatus& simulated_valve_status;
  
  public:

    Servo loadingValveServo;

    ControlModule(SimulatedValveStatus& simulated_valve_status_val) : simulated_valve_status(simulated_valve_status_val) {}

    void init() {
      // Allow allocation of all timers
        
      delay(500);

      pinMode(NOX_UMBRILICAL_PIN, OUTPUT);
      pinMode(LOADING_VALVE_PIN, OUTPUT);
      pinMode(IGNITERS_PIN, OUTPUT);
      pinMode(ENGINE_VALVE_PIN, OUTPUT);
      digitalWrite(NOX_UMBRILICAL_PIN, HIGH);
      digitalWrite(LOADING_VALVE_PIN, HIGH);
      digitalWrite(IGNITERS_PIN, HIGH);
      digitalWrite(ENGINE_VALVE_PIN, HIGH);

      // move_relay(NOX_UMBRILICAL_PIN, Open);
      // delay(500);
      // move_relay(NOX_UMBRILICAL_PIN, Close);
      // delay(500);
      // move_relay(IGNITERS_PIN, Open);
      // delay(500);
      // move_relay(IGNITERS_PIN, Close);
      // delay(500);
      // move_relay(LOADING_VALVE_PIN, Open);
      // delay(500);
      // move_relay(LOADING_VALVE_PIN, Close);
      // delay(500);
      // move_relay(ENGINE_VALVE_PIN, Open);
      // delay(500);
      // move_relay(ENGINE_VALVE_PIN, Close);

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
        case ENGINE_VALVE:
          move_relay(ENGINE_VALVE_PIN, command.uint_value);
          simulated_valve_status.engine_valve_open = command.uint_value > 127.5;
          break;
      }
    }

    void set_igniters_on(bool value) {
      Logger::debug("Control.set_igniters_on" + value);
      move_relay(IGNITERS_PIN, value ? 255 : 0);
    }
    
    void disconnect_umbrilical(bool value) {
      Logger::debug("Control.disconnect_umbrilical");
      move_relay(NOX_UMBRILICAL_PIN, value ? 255 : 0);
    }

    void move_relay(uint8_t pin, uint8_t value) {
      digitalWrite(pin, value > 127.5 ? LOW : HIGH);
    }

};
