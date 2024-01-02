#include "Logger.h"
#include "ControlModule.h"
#include "ElectromechanicalModule.h"
#include "CommunicationModule.h"
#include "SensorModule.h"
#include "WeatherModule.h"
#include "SystemStatus.h"
#include <stdint.h>
#include <string>
#include <format>
#include <vector>


const int MAX_LOADING_LINE_PRESSURE_PSI = 100;
const int MAX_TANK_PRESSURE_PSI = 100;
const unsigned long MAX_PRESSURE_WARNING_TIME_MILLIS = 20000;
const unsigned long MAX_PRESSURE_WARNING_TIME_EXT_VENT_MILLIS = 20000;
const int MAX_LAUNCH_WIND_KT = 12;

const float TANK_DEPRESS_VENT_LOW_TEMP = 10;

const float OBEC_LOWEST_BATTERY_VOLTAGE = 4;


// System status variables
SystemStatus system_status = SystemStatus();
unsigned long last_milis = 0;
unsigned long preflight_check_time = 0;
unsigned long standby_pressure_warning_start_time = 0;
unsigned long pre_launch_start_time = 0;
unsigned long umbrilical_disconnect_time = 0;

// Modules
StorageModule storage_module = StorageModule();
ControlModule control_module = ControlModule();
SensorModule sensor_module = SensorModule();
CommunicationModule communication_module = CommunicationModule();
WeatherModule weather_module = WeatherModule();

void setup()
{
  Serial.begin(9600);

  storage_module.init();
  sensor_module.init();
  electromechanical_module.init();
  communication_module.init();

  lastMilis = millis();

  switchToState(storageModule.loadCurrentState(STATUS));
}

void loop()
{
  unsigned long currMillis = millis();

  switch (currentState)
  {
  case STANDBY:
    if (currMillis - lastMilis > 100)
    {
      if (system_status.tank_pressure_psi > TANK_PRESSURE_WARN_PSI) {
        if (system_status.external_vent_as_default) {
          switch_to_state(STANDBY_PRESSURE_WARNING_EXTERNAL_VENT);
        } else {
          switch_to_state(STANDBY_PRESSURE_WARNING);
        }
      }
      lastMilis = currMillis;
    }
    break;
  case LOADING:
    if (currMillis - lastMilis > 100)
    {
      if (system_status.tank_depress_vent_temperature_celsius < TANK_DEPRESS_VENT_LOW_TEMP) {
        control_module.execute_valve_command(new ValveCommand(Close, LOADING_VALVE));
        communication_module.send_valve_command_to_OBEC(new ValveCommand(Close, TANK_DEPRESS_VENT_VALVE));
        switch_to_state(STANDBY);
      } 
      
      lastMilis = currMillis;
    }
    break;
  case STANDBY_PRESSURE_WARNING:
    if (currMillis - lastMilis > 500)
    {
      if (system_status.tank_pressure_psi > TANK_PRESSURE_WARN_PSI) {
        if (millis() - standby_pressure_warning_start_time > MAX_PRESSURE_WARNING_TIME_MILLIS) {
          switch_to_state(STANDBY_PRESSURE_WARNING_EXTERNAL_VENT);
        }
      } else {
        switch_to_state(STANDBY);
      }
      lastMilis = currMillis;
    }
    break;
  case STANDBY_PRESSURE_WARNING_EXTERNAL_VENT:
    if (currMillis - lastMilis > 500)
    {
      if (system_status.tank_pressure_psi > TANK_PRESSURE_WARN_PSI) {
        if (millis() - standby_pressure_warning_start_time > MAX_PRESSURE_WARNING_TIME_EXT_VENT_MILLIS) {
          switch_to_state(ABORT);
        }
      } else {
        system_status.external_vent_as_default = true;
        communication_module.send_external_vent_as_default_prompt_to_MCC();
        switch_to_state(STANDBY);
      }
      lastMilis = currMillis;
    }
    break;
    case PRE_LAUNCH_WIND_CHECK:
     if (currMillis - lastMilis > 500)
     {
        if (weather_module.get_wind_kt() > MAX_LAUNCH_WIND_KT) {
          communication_module.send_wind_abort_to_MCC();
          switch_to_state(STANDBY);
        } else if (millis() - prelaunch_start_time > 5000) {
          switch_to_state(PRE_LAUNCH_UMBRILICAL_DISCONNECT);
        } 
        lastMilis = currMillis;
      }
      break;
    case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
      if (milis() - umbrilical_disconnect_time > 600)
      {
        if (!sensor_module.umbrilical_connected()) {
          communication_module.send_umbrilical_abort_to_MCC();
          break;
        }
        if (milis() - umbrilical_disconnect_time > 10000) {
          switch_to_state(IGNITION);
        }
      }
      break;
    case IGNITION: 
      break;
  }
}

void switch_to_state(State newState)
{
  system_status.currentState = newState;
  Logger::log("Switching to state: " + String(currentState));
  storageModule.saveCurrentState(system_status.currentState);
  switch (system_status.currentState)
  {
    case STANDBY_PRESSURE_WARNING:
      communication_module.send_valve_command_to_OBEC(new ValveCommand(Open, TANK_DEPRESS_VENT_VALVE));
      standby_pressure_warning_start_time = millis();
      delay(1000) // TODO: Handle these delays without blocking the processor
      break;
    case STANDBY_PRESSURE_WARNING_EXTERNAL_VENT:
      
      break;
    case LOADING:
      if (system_status.loading_line_pressure_psi < MAX_LOADING_LINE_PRESSURE_PSI &&
            system_status.tank_pressure_psi < TANK_PRESSURE_WARN_PSI){
        communication_module.send_valve_command_to_OBEC(new ValveCommand(Close, ENGINE_VALVE));
        control_module.execute_valve_command(new ValveCommand(Close, LOADING_LINE_DEPRESS_VENT_VALVE));
        communication_module.send_valve_command_to_OBEC(new ValveCommand(Open, TANK_DEPRESS_VENT_VALVE));
        if (system_status.tank_depress_vent_temperature_celsius < TANK_DEPRESS_VENT_LOW_TEMP) {
          communication_module.send_tank_depress_vent_temp_low_to_MCC();
          switch_to_state(STANDBY);
        } else {
          control_module.execute_valve_command(new ValveCommand(Open, LOADING_LINE_VALVE));
        }
      }
      break;
    case PREFLIGHT_CHECK:
      PreflightCheckData preflight_check_data = new PreflightCheckData();
      preflight_check_data.tank_pressure_psi = system_status.tank_pressure_psi;
      preflight_check_data.tank_temperature_celsius = system_status.tank_temperature_celsius;
      preflight_check_data.engine_valve_open = system_status.engine_valve_open;
      preflight_check_data.loading_valve_open = system_status.loading_valve_open;
      control_module.set_obec_power(false);
      delay(2000);
      preflight_check_data.obec_battery_voltage_volt = system_status.obec_battery_voltage_volt;
      if (preflight_check_data.obec_battery_voltage_volt > OBEC_LOWEST_BATTERY_VOLTAGE) {
        preflight_check_data.lc_battery_voltage_volt = system_status.lc_battery_voltage_volt;
        preflight_check_data.flight_computers_status = new FlightComputerStatus(true, true, true); // TODO: CHECK FLIGHT COMPUTERS STATUS
        preflight_check_data.igniter_continuity_ok = system_status.igniter_continuity_ok;
        preflight_check_data.weather_data = new WeatherData(weather_module.get_wind());  
      } else {
        control_module.set_obec_power(true);
      }
      communication_module.send_preflight_check_data_to_MCC(preflight_check_data);
      preflight_check_time = millis();
      switch_to_state(STANDBY);
      break;
    case PRE_LAUNCH_WIND_CHECK:
      if (millis() - preflight_check_time > 10000) {
        communication_module.send_stale_flight_check_to_MCC();
        switch_to_state(STANDBY);
      } else {
        pre_launch_start_time = milllis();  
      }
      break;
    case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
      control_module.disconnect_umbrilical();
      umbrilical_disconnect_time = milis();
      break;
    case ABORT:
      communication_module.send_valve_command_to_OBEC(new ValveCommand(Open, TANK_DEPRESS_VENT_VALVE));
      control_module.execute_valve_command(new ValveCommand(Open, LOADING_LINE_DEPRESS_VENT_VALVE))
      communication_module.send_valve_command_to_OBEC(new ValveCommand(Close, ENGINE_VALVE));
      control_module.execute_valve_command(new ValveCommand(Close, LOADING_VALVE));
      control_module.set_igniters_on(false);
      communication_module.send_abort_signal_to_OBEC();
      break;
  }
}

// Format is <PACKET_COUNT>,<STATE>,<ALTITUDE>,<ACCELERATION>
// Example result (minus the spaces): STBY 0014 10.1 25.2 0014 4.12 4.20 ? 008
string create_LC_status_packet_str(SystemStatus status)
{
  Logger::debug("Creating telemetry packet...");
  string state_str = get_state_string(status.current_state);
  string tank_pressure_str = std::format("{:0{}}", status.tank_pressure_psi, 4); // 4 is the desired number of digits (using leading zeroes)
  string tank_temperature_str = std::format("{:.1f}", status.tank_temperature_celsius);
  string loading_line_pressure_str = std::format("{:0{}}", status.loading_line_pressure_psi, 4); // 4 is the desired number of digits (using leading zeroes)
  string obec_battery_voltage_str = std::format("{:.12f}", status.obec_battery_voltage_volt);
  string lc_battery_voltage_str = std::format("{:.12f}", status.lc_battery_voltage_volt);
  string status_flags_str = string(create_status_flags_byte(status.obec_connection_ok,
                                                            status.tank_depress_vent_valve_open,
                                                            status.engine_valve_open,
                                                            status.loading_valve_open,
                                                            status.loading_depress_vent_valve_open,
                                                            status.hydraulic_umbrilical_connected,
                                                            status.igniter_continuity_ok), 1);
  string wind_str = std::format("{:0{}}", wind_kt, 3); // 3 is the desired number of digits (using leading zeroes)

  return state_str +
          tank_pressure_str +
          tank_temperature_str +
          loading_line_pressure_str +
          obec_battery_voltage_str +
          lc_battery_voltage_str +
          status_flags_str +
          wind_str;
}

char create_status_flags_byte(bool obec_connection_ok,
                                   bool tank_depress_vent_valve_open,
                                   bool engine_valve_open,
                                   bool loading_valve_open,
                                   bool loading_depress_vent_valve_open,
                                   bool hydraulic_umbrilical_connected,
                                   bool igniter_continuity_ok) {
    char result = 0;
    if (obec_connection_ok) result++;
    result<<1;
    if (tank_depress_vent_valve_open) result++;
    result<<1;
    if (engine_valve_open) result++;
    result<<1;
    if (loading_valve_open) result++;
    result<<1;
    if (loading_depress_vent_valve_open) result++;
    result<<1;
    if (hydraulic_umbrilical_connected) result++;
    result<<1;
    if (igniter_continuity_ok) result++;
    return result;
}