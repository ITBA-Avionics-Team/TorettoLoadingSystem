#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

#include "TorettoLib.h"

// #ifndef OLED_H
// #define OLED_H
// #include "OLEDModule.h"
// #endif // OLED_H

// #define SIMULATION_MODULE true
// #define SIMULATED_SENSOR_MODULE true
// #define SIMULATED_COMMUNICATION_MODULE true

#include "modules/ControlModule.h"
#include "modules/WeatherModule.h"

#include <stdint.h>
#include <string.h>


const int LOADING_LINE_PRESSURE_WARN_BAR = 200;
const int TANK_PRESSURE_WARN_PSI = 100;
const unsigned long MAX_PRESSURE_WARNING_TIME_MILLIS = 20000;
const unsigned long MAX_PRESSURE_WARNING_TIME_EXT_VENT_MILLIS = 30000;
const int MAX_LAUNCH_WIND_KT = 12;

const float TANK_DEPRESS_VENT_LOW_TEMP = 10;

const float OBEC_LOWEST_BATTERY_VOLTAGE = 4;
const unsigned long OBEC_CONNECTION_LOST_TIMEOUT_MILLIS = 3000;


// System status variables
SystemStatus system_status = SystemStatus();
unsigned long last_milis = 0;
unsigned long last_mcc_update_milis = 0;
unsigned long last_obec_status_received_milis = 0;
unsigned long simulation_mode_update_millis = 0;
unsigned long preflight_check_time = 0;
unsigned long standby_pressure_warning_start_time = 0;
unsigned long pre_launch_start_time = 0;
unsigned long umbrilical_disconnect_time = 0;
unsigned long ignition_start_time = 0;
unsigned long igniters_on_time = 0;

// Modules
#ifdef SIMULATION_MODULE
#include "simulation_modules/SimulationModule.h"
SimulationModule simulation_module = SimulationModule();
#endif

SimulatedValveStatus simulated_valve_status = SimulatedValveStatus();

ControlModule control_module = ControlModule(simulated_valve_status);


#ifdef SIMULATED_SENSOR_MODULE
#include "simulation_modules/SimulatedSensorModule.h"
SimulatedSensorModule sensor_module = SimulatedSensorModule(simulation_module);
#else
#include "modules/SensorModule.h"
SensorModule sensor_module = SensorModule(simulated_valve_status);
#endif


#ifdef SIMULATED_COMMUNICATION_MODULE
#include "simulation_modules/SimulatedCommunicationModule.h"
SimulatedCommunicationModule communication_module = SimulatedCommunicationModule(simulation_module);
#else
#include "modules/CommunicationModule.h"
CommunicationModule communication_module = CommunicationModule();
#endif

WeatherModule weather_module = WeatherModule();



void setup()
{
  Serial.begin(115200, SERIAL_8N1, 3, 1);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // Logger::blink_debug_led_times(5);
  
  control_module.init();
  sensor_module.init();
  communication_module.init();

  last_milis = millis();

  switch_to_state(STANDBY);
}

void loop() {
  unsigned long currMilis = millis();


#ifdef SIMULATION_MODULE
  if (currMilis - simulation_mode_update_millis > 200) {
    simulation_module.loop();  
    simulation_mode_update_millis = currMilis;
  }
#endif
  

  if (currMilis - last_mcc_update_milis > 300) {
    communication_module.check_for_MCC_commands();
    communication_module.check_for_OBEC_status();

    if (communication_module.get_new_MCC_command_available()) {
      Command command = communication_module.get_latest_MCC_command();
      switch (command.type) {
        case ValveCommand:
        // Serial.println("Received command");
          control_module.execute_valve_command(command);
          break;
        case SwitchStateCommand:
          switch_to_state(command.state);
          break;
      }
      communication_module.set_new_MCC_command_available(false);
    }

    if (communication_module.get_new_OBEC_status_available()) {
      last_obec_status_received_milis = millis();
      OBECStatus obec_status = communication_module.get_latest_OBEC_status();
      system_status.tank_depress_vent_temperature_celsius = obec_status.tank_depress_vent_temperature_celsius;
      system_status.obec_battery_voltage_volt = obec_status.obec_battery_voltage_volt;

      communication_module.set_new_OBEC_status_available(false);
    }

    update_sensor_and_weather_data();

    communication_module.send_system_status_to_MCC(system_status);

    last_mcc_update_milis = currMilis;
  }

  switch (system_status.current_state) {
    case STANDBY:
      if (currMilis - last_milis > 100) {
        if (system_status.loading_line_pressure_bar > LOADING_LINE_PRESSURE_WARN_BAR) {
          switch_to_state(STANDBY_PRESSURE_WARNING);
        }
        last_milis = currMilis;
      }
      break;
    case LOADING:
      if (currMilis - last_milis > 100) {
        if (system_status.tank_depress_vent_temperature_celsius < TANK_DEPRESS_VENT_LOW_TEMP && system_status.tank_depress_vent_temperature_celsius != -17) {
          control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_VALVE));
          switch_to_state(STANDBY);
        }
        last_milis = currMilis;
      }
      break;
    case STANDBY_PRESSURE_WARNING:
      if (currMilis - last_milis > 500) {
        if (system_status.loading_line_pressure_bar > LOADING_LINE_PRESSURE_WARN_BAR) {
          if (millis() - standby_pressure_warning_start_time > MAX_PRESSURE_WARNING_TIME_MILLIS) {
            switch_to_state(ABORT);
          }
        } else {
          switch_to_state(STANDBY);
        }
        last_milis = currMilis;
      }
      break;
    case PRE_LAUNCH_WIND_CHECK:
      if (currMilis - last_milis > 500) {
        if (weather_module.get_wind_kt() > MAX_LAUNCH_WIND_KT) {
          communication_module.send_wind_abort_to_MCC();
          switch_to_state(STANDBY);
        } else if (millis() - pre_launch_start_time > 5000) {
          switch_to_state(PRE_LAUNCH_UMBRILICAL_DISCONNECT);
        }
        last_milis = currMilis;
      }
      break;
    case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
      if (currMilis - umbrilical_disconnect_time > 5000) {
        control_module.disconnect_umbrilical(false);
        switch_to_state(STANDBY);
      }
      break;
    case IGNITION_OPEN_VALVE:
      if (currMilis - ignition_start_time > 1000) {
        switch_to_state(IGNITION_IGNITERS_ON);
      }
      break;
    case IGNITION_IGNITERS_ON:
      if (currMilis - igniters_on_time > 10000) {
        control_module.set_igniters_on(false);
        switch_to_state(STANDBY);
      }
      break;
    case IGNITION_IGNITERS_OFF:
      if (currMilis - ignition_start_time > 4000) {
        control_module.set_igniters_on(false);
        switch_to_state(STANDBY);
      }
      break;
  }
}

void switch_to_state(State newState) {
  system_status.current_state = newState;
  Logger::log("Switching to state: " + get_state_complete_string(system_status.current_state));
  switch (system_status.current_state) {
    case STANDBY_PRESSURE_WARNING:
      standby_pressure_warning_start_time = millis();
      break;
    case LOADING:
      if (system_status.loading_line_pressure_bar < LOADING_LINE_PRESSURE_WARN_BAR) {
        control_module.execute_valve_command(Command(ValveCommand, Close, ENGINE_VALVE));
        control_module.execute_valve_command(Command(ValveCommand, Open, LOADING_VALVE));
      } else {
        switch_to_state(STANDBY_PRESSURE_WARNING);
      }
      break;
    case PRE_FLIGHT_CHECK:
      {
        FlightComputersStatus status = FlightComputersStatus(true, true, true);
        communication_module.send_flight_computer_status_to_MCC(status);
        preflight_check_time = millis();
        switch_to_state(STANDBY);
        break;
      }
    case PRE_LAUNCH_WIND_CHECK:
      if (millis() - preflight_check_time > 10000) {
        communication_module.send_stale_flight_check_to_MCC();
        switch_to_state(STANDBY);
      } else {
        pre_launch_start_time = millis();
      }
      break;
    case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
      control_module.disconnect_umbrilical(true);
      umbrilical_disconnect_time = millis();
      break;
    case IGNITION_OPEN_VALVE:
      ignition_start_time = millis();
      control_module.execute_valve_command(Command(ValveCommand, Open, ENGINE_VALVE));
      break;
    case IGNITION_IGNITERS_ON:
      igniters_on_time = millis();
      control_module.set_igniters_on(true);
      break;
    case IGNITION_IGNITERS_OFF:
      control_module.set_igniters_on(false);
      communication_module.send_ignition_confirmation_to_MCC();
      break;
    case ABORT:
      control_module.execute_valve_command(Command(ValveCommand, Close, ENGINE_VALVE));
      control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_VALVE));
      control_module.set_igniters_on(false);
      break;
    default:
      break;
  }
}

void update_sensor_and_weather_data() {
  system_status.loading_line_pressure_bar = sensor_module.get_loading_line_pressure_bar();
  system_status.ground_pressure_bar = sensor_module.get_ground_pressure_bar();
  system_status.ground_temperature_celsius = sensor_module.get_ground_temperature_celsius();
  system_status.obec_connection_ok = millis() - last_obec_status_received_milis < OBEC_CONNECTION_LOST_TIMEOUT_MILLIS;
  system_status.engine_valve_open = sensor_module.get_engine_valve_open();
  system_status.loading_valve_open = sensor_module.get_loading_valve_open();
  system_status.hydraulic_umbrilical_connected = sensor_module.get_hydraulic_umbrilical_connected();
  system_status.hydraulic_umbrilical_finished_disconnect = sensor_module.get_hydraulic_umbrilical_finished_disconnect();
  system_status.wind_kt = weather_module.get_wind_kt();
}
