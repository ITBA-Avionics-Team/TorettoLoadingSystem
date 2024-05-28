#define LED_PIN 25
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

// #ifndef OLED_H
// #define OLED_H
// #include "OLEDModule.h"
// #endif // OLED_H

#define SIMULATION_MODULE true
// #define SIMULATED_SENSOR_MODULE true
// #define SIMULATED_COMMUNICATION_MODULE true

#include "modules/StorageModule.h"
#include "modules/ControlModule.h"
#include "modules/WeatherModule.h"

#include <stdint.h>
#include <string.h>


const int MAX_LOADING_LINE_PRESSURE_PSI = 100;
const int TANK_PRESSURE_WARN_PSI = 100;
const unsigned long MAX_PRESSURE_WARNING_TIME_MILLIS = 20000;
const unsigned long MAX_PRESSURE_WARNING_TIME_EXT_VENT_MILLIS = 30000;
const int MAX_LAUNCH_WIND_KT = 12;

const float TANK_DEPRESS_VENT_LOW_TEMP = 10;

const float OBEC_LOWEST_BATTERY_VOLTAGE = 4;


// System status variables
SystemStatus system_status = SystemStatus();
unsigned long last_milis = 0;
unsigned long last_mcc_update_milis = 0;
unsigned long simulation_mode_update_millis = 0;
unsigned long preflight_check_time = 0;
unsigned long standby_pressure_warning_start_time = 0;
unsigned long pre_launch_start_time = 0;
unsigned long umbrilical_disconnect_time = 0;
unsigned long ignition_start_time = 0;

// Modules
#ifdef SIMULATION_MODULE
#include "simulation_modules/SimulationModule.h"
SimulationModule simulation_module = SimulationModule();
#endif

SimulatedValveStatus simulated_valve_status = SimulatedValveStatus();

StorageModule storage_module = StorageModule();
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

pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
  // OLEDModule::init();
  

  storage_module.init();
  control_module.init();
  sensor_module.init();
  // electromechanical_modul  e.init();
  // communication_module.init();

  last_milis = millis();

  // switch_to_state(storage_module.load_current_state(STANDBY));
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
          if (is_LC_valve(command.valve)) {
            // digitalWrite(LED_PIN, HIGH);
            control_module.execute_valve_command(command);
          } else {
            communication_module.send_valve_command_to_OBEC(command);
          }
          break;
        case SwitchStateCommand:
          switch_to_state(command.state);
          break;
        case SetExternalVentAsDefaultCommand:
          system_status.external_vent_as_default = command.bool_value;
          break;
      }
      communication_module.set_new_MCC_command_available(false);
    }

    if (communication_module.get_new_OBEC_status_available()) {
      // Logger::log("[Main]new OBEC status available");
      OBECStatus obec_status = communication_module.get_latest_OBEC_status();
      system_status.tank_pressure_bar = obec_status.tank_pressure_bar;
      system_status.tank_temperature_celsius = obec_status.tank_temperature_celsius;
      system_status.tank_depress_vent_temperature_celsius = obec_status.tank_depress_vent_temperature_celsius;
      system_status.obec_battery_voltage_volt = obec_status.obec_battery_voltage_volt;
      system_status.tank_depress_vent_valve_open = obec_status.tank_depress_vent_valve_open;
      system_status.engine_valve_open = obec_status.engine_valve_open;

      communication_module.set_new_OBEC_status_available(false);
    }

    update_sensor_and_weather_data();

    communication_module.send_system_status_to_MCC(system_status);

    last_mcc_update_milis = currMilis;
  }

  switch (system_status.current_state) {
    case STANDBY:
      if (currMilis - last_milis > 100) {
        if (system_status.tank_pressure_bar > TANK_PRESSURE_WARN_PSI) {
          if (system_status.external_vent_as_default) {
            switch_to_state(STANDBY_PRESSURE_WARNING_EXTERNAL_VENT);
          } else {
            switch_to_state(STANDBY_PRESSURE_WARNING);
          }
        }
        last_milis = currMilis;
      }
      break;
    case LOADING:
      if (currMilis - last_milis > 100) {
        if (system_status.tank_depress_vent_temperature_celsius < TANK_DEPRESS_VENT_LOW_TEMP) {
          control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_VALVE));
          communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Close, TANK_DEPRESS_VENT_VALVE));
          switch_to_state(STANDBY);
        }

        last_milis = currMilis;
      }
      break;
    case STANDBY_PRESSURE_WARNING:
      if (currMilis - last_milis > 500) {
        if (system_status.tank_pressure_bar > TANK_PRESSURE_WARN_PSI) {
          if (millis() - standby_pressure_warning_start_time > MAX_PRESSURE_WARNING_TIME_MILLIS) {
            switch_to_state(STANDBY_PRESSURE_WARNING_EXTERNAL_VENT);
          }
        } else {
          switch_to_state(STANDBY);
        }
        last_milis = currMilis;
      }
      break;
    case STANDBY_PRESSURE_WARNING_EXTERNAL_VENT:
      if (currMilis - last_milis > 500) {
        if (system_status.tank_pressure_bar > TANK_PRESSURE_WARN_PSI) {
          if (millis() - standby_pressure_warning_start_time > MAX_PRESSURE_WARNING_TIME_EXT_VENT_MILLIS) {
            switch_to_state(ABORT);
          }
        } else {
          system_status.external_vent_as_default = true;
          communication_module.send_external_vent_as_default_prompt_to_MCC();
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
        if (sensor_module.get_hydraulic_umbrilical_connected()) {
          communication_module.send_umbrilical_abort_to_MCC();
          switch_to_state(STANDBY);
          break;
        }
        if (currMilis - umbrilical_disconnect_time > 10000) {
          switch_to_state(IGNITION_IGNITERS_ON);
        }
      }
      break;
    case IGNITION_IGNITERS_ON:
      if (currMilis - ignition_start_time > 500) {
        switch_to_state(IGNITION_OPEN_VALVE);
      }
    case IGNITION_OPEN_VALVE:
      if (currMilis - ignition_start_time < 3000) {
        communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Open, ENGINE_VALVE));  // TODO: I think we should open this valve gradually, which implies a modification of the ValveComand structure.
        // Value between 0 and 255 would look something like
        // (((currMilis - ignition_start_time) - 500) / 2500) * 255
        // We might want to make this discrete by using last_milis like we do in other states.
        
        // Open bleed 3% for 1000ms (1 seconds) before igniters
        // Igniters on for 3 seconds
        // Open 100% and leave open
        // 1 second later Igniters off total 4 secs of gniters

      } else {
        switch_to_state(IGNITION_IGNITERS_OFF);
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
  storage_module.saveCurrentState(system_status.current_state);
  switch (system_status.current_state) {
    case STANDBY:
      control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_LINE_DEPRESS_VENT_VALVE));
      communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Close, TANK_DEPRESS_VENT_VALVE));
    case STANDBY_PRESSURE_WARNING:
      communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Open, TANK_DEPRESS_VENT_VALVE));
      standby_pressure_warning_start_time = millis();
      break;
    case STANDBY_PRESSURE_WARNING_EXTERNAL_VENT:
      control_module.execute_valve_command(Command(ValveCommand, Open, LOADING_LINE_DEPRESS_VENT_VALVE));
      break;
    case LOADING:
      if (system_status.loading_line_pressure_bar < MAX_LOADING_LINE_PRESSURE_PSI && system_status.tank_pressure_bar < TANK_PRESSURE_WARN_PSI) {
        communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Close, ENGINE_VALVE));
        control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_LINE_DEPRESS_VENT_VALVE));
        communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Open, TANK_DEPRESS_VENT_VALVE));
        if (system_status.tank_depress_vent_temperature_celsius < TANK_DEPRESS_VENT_LOW_TEMP) { // TODO: This measurement probably isn't reliable if we check the temperature as soon as we send the signal to open the valve.
          communication_module.send_tank_depress_vent_temp_low_to_MCC();
          switch_to_state(STANDBY);
        } else {
          control_module.execute_valve_command(Command(ValveCommand, Open, LOADING_VALVE));
        }
      }
      break;
    case PRE_FLIGHT_CHECK:
      {
        PreflightCheckData preflight_check_data = PreflightCheckData();
        preflight_check_data.tank_pressure_bar = system_status.tank_pressure_bar;
        preflight_check_data.tank_temperature_celsius = system_status.tank_temperature_celsius;
        preflight_check_data.engine_valve_open = system_status.engine_valve_open;
        preflight_check_data.loading_valve_open = system_status.loading_valve_open;
        control_module.set_obec_power(false);
        delay(2000);
        preflight_check_data.obec_battery_voltage_volt = system_status.obec_battery_voltage_volt;
        if (preflight_check_data.obec_battery_voltage_volt > OBEC_LOWEST_BATTERY_VOLTAGE) {
          preflight_check_data.lc_battery_voltage_volt = system_status.lc_battery_voltage_volt;
          preflight_check_data.flight_computers_status = FlightComputersStatus(true, true, true);  // TODO: CHECK FLIGHT COMPUTERS STATUS
          preflight_check_data.igniter_continuity_ok = system_status.igniter_continuity_ok;
          preflight_check_data.weather_data = WeatherData(weather_module.get_wind_kt());
        } else {
          control_module.set_obec_power(true);
        }
        communication_module.send_preflight_check_data_to_MCC(preflight_check_data);
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
      control_module.disconnect_umbrilical();
      umbrilical_disconnect_time = millis();
      break;
    case IGNITION_IGNITERS_ON:
      control_module.set_igniters_on(true);
      ignition_start_time = millis();
      break;
    case IGNITION_IGNITERS_OFF:
      communication_module.send_ignition_confirmation_to_OBEC();
      communication_module.send_ignition_confirmation_to_MCC();
      break;
    case ABORT:
      communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Open, TANK_DEPRESS_VENT_VALVE));
      control_module.execute_valve_command(Command(ValveCommand, Open, LOADING_LINE_DEPRESS_VENT_VALVE));
      communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Close, ENGINE_VALVE));
      control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_VALVE));
      control_module.set_igniters_on(false);
      communication_module.send_abort_signal_to_OBEC();
      break;
    default:
      break;
  }
}

void update_sensor_and_weather_data() {
  system_status.loading_line_pressure_bar = sensor_module.get_loading_line_pressure_bar();
  system_status.lc_battery_voltage_volt = sensor_module.get_lc_battery_voltage_volt();
  system_status.obec_connection_ok = sensor_module.get_obec_connection_ok();
  system_status.loading_valve_open = sensor_module.get_loading_valve_open();
  system_status.loading_depress_vent_valve_open = sensor_module.get_loading_depress_vent_valve_open();
  system_status.hydraulic_umbrilical_connected = sensor_module.get_hydraulic_umbrilical_connected();
  system_status.igniter_continuity_ok = sensor_module.get_igniter_continuity_ok();
  system_status.wind_kt = weather_module.get_wind_kt();
  system_status.tank_temperature_celsius = sensor_module.get_tank_temperature_celsius();
}
