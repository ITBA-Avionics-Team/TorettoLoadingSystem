#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H
#define SIMULATION_MODULE true
#define SIMULATED_COMMUNICATION_MODULE true

#include "modules/StorageModule.h"
#include "modules/ControlModule.h"
#include "modules/SensorModule.h"
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

StorageModule storage_module = StorageModule();
ControlModule control_module = ControlModule();
SensorModule sensor_module = SensorModule();

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
  Serial.begin(19200);

  storage_module.init();
  // sensor_module.init();
  // electromechanical_module.init();
  // communication_module.init();

  last_milis = millis();

  switch_to_state(storage_module.load_current_state(STANDBY));
}

void loop()
{
  #ifdef SIMULATED_COMMUNICATION_MODULE
  simulation_module.loop();
  #endif

  unsigned long currMilis = millis();

  if (currMilis - last_mcc_update_milis > 300) {
    communication_module.check_for_MCC_commands();
    communication_module.check_for_OBEC_status();
  
    if (communication_module.get_new_MCC_command_available()) {
      Command command = communication_module.get_latest_MCC_command();
      switch(command.type){
        case ValveCommand:
          if (is_LC_valve(command.valve)){
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
      OBECStatus obec_status = communication_module.get_latest_OBEC_status();
      system_status.tank_pressure_psi = obec_status.tank_pressure_psi;
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

  switch (system_status.current_state)
  {
  case STANDBY:
    if (currMilis - last_milis > 100)
    {
      if (system_status.tank_pressure_psi > TANK_PRESSURE_WARN_PSI) {
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
    if (currMilis - last_milis > 100)
    {
      if (system_status.tank_depress_vent_temperature_celsius < TANK_DEPRESS_VENT_LOW_TEMP) {
        control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_VALVE));
        communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Close, TANK_DEPRESS_VENT_VALVE));
        switch_to_state(STANDBY);
      } 
      
      last_milis = currMilis;
    }
    break;
  case STANDBY_PRESSURE_WARNING:
    if (currMilis - last_milis > 500)
    {
      if (system_status.tank_pressure_psi > TANK_PRESSURE_WARN_PSI) {
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
    if (currMilis - last_milis > 500)
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
      last_milis = currMilis;
    }
    break;
    case PRE_LAUNCH_WIND_CHECK:
     if (currMilis - last_milis > 500)
     {
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
      if (currMilis - umbrilical_disconnect_time > 600)
      {
        if (!sensor_module.get_hydraulic_umbrilical_connected()) {
          communication_module.send_umbrilical_abort_to_MCC();
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
        communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Open, ENGINE_VALVE)); // TODO: I think we should open this valve gradually, which implies a modification of the ValveComand structure.
        // Value between 0 and 255 would look something like
        // (((currMilis - ignition_start_time) - 500) / 2500) * 255
        // We might want to make this discrete by using last_milis like we do in other states.
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

void switch_to_state(State newState)
{
  system_status.current_state = newState;
  Logger::log("Switching to state: " + String(system_status.current_state));
  storage_module.saveCurrentState(system_status.current_state);
  switch (system_status.current_state)
  {
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
      if (system_status.loading_line_pressure_psi < MAX_LOADING_LINE_PRESSURE_PSI &&
            system_status.tank_pressure_psi < TANK_PRESSURE_WARN_PSI){
        communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Close, ENGINE_VALVE));
        control_module.execute_valve_command(Command(ValveCommand, Close, LOADING_LINE_DEPRESS_VENT_VALVE));
        communication_module.send_valve_command_to_OBEC(Command(ValveCommand, Open, TANK_DEPRESS_VENT_VALVE));
        if (system_status.tank_depress_vent_temperature_celsius < TANK_DEPRESS_VENT_LOW_TEMP) {
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
      preflight_check_data.tank_pressure_psi = system_status.tank_pressure_psi;
      preflight_check_data.tank_temperature_celsius = system_status.tank_temperature_celsius;
      preflight_check_data.engine_valve_open = system_status.engine_valve_open;
      preflight_check_data.loading_valve_open = system_status.loading_valve_open;
      control_module.set_obec_power(false);
      delay(2000);
      preflight_check_data.obec_battery_voltage_volt = system_status.obec_battery_voltage_volt;
      if (preflight_check_data.obec_battery_voltage_volt > OBEC_LOWEST_BATTERY_VOLTAGE) {
        preflight_check_data.lc_battery_voltage_volt = system_status.lc_battery_voltage_volt;
        preflight_check_data.flight_computers_status = FlightComputersStatus(true, true, true); // TODO: CHECK FLIGHT COMPUTERS STATUS
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

// Format is <state><tank_p><tank_t><tank_depress_vent_t><loading_line_p><obec_voltage><lc_voltage><sensor_data_byte><wind>
// Example result (minus the spaces): STBY 0014 10.1 25.2 0014 4.12 4.20 ? 008
String create_LC_status_packet_str(SystemStatus status)
{
  char buffer[10];
  Logger::debug("Creating telemetry packet...");
  String state_str = get_state_string(status.current_state);
  sprintf(buffer, "%0*d", 4, status.tank_pressure_psi); // 4 is the desired number of digits (using leading zeroes)
  String tank_pressure_str(buffer);
  sprintf(buffer, "%.1f", status.tank_temperature_celsius);
  String tank_temperature_str(buffer);
  sprintf(buffer, "%0*d", 4, status.loading_line_pressure_psi); // 4 is the desired number of digits (using leading zeroes)
  String loading_line_pressure_str(buffer); 
  sprintf(buffer, "%.12f", status.obec_battery_voltage_volt);
  String obec_battery_voltage_str(buffer);
  sprintf(buffer, "%.12f", status.lc_battery_voltage_volt);
  String lc_battery_voltage_str(buffer);
  String status_flags_str = String(create_status_flags_byte(status.obec_connection_ok,
                                                            status.tank_depress_vent_valve_open,
                                                            status.engine_valve_open,
                                                            status.loading_valve_open,
                                                            status.loading_depress_vent_valve_open,
                                                            status.hydraulic_umbrilical_connected,
                                                            status.igniter_continuity_ok), 1);
  
  sprintf(buffer, "%0*d", 3, status.wind_kt); // 3 is the desired number of digits (using leading zeroes)
  String wind_str(buffer);

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

String get_state_string(State state) {
  return "STBY"; //TODO: Implement
}

void update_sensor_and_weather_data() {
  system_status.loading_line_pressure_psi = sensor_module.get_loading_line_pressure_psi();
  system_status.lc_battery_voltage_volt = sensor_module.get_lc_battery_voltage_volt();
  system_status.obec_connection_ok = sensor_module.get_obec_connection_ok();
  system_status.loading_valve_open = sensor_module.get_loading_valve_open();
  system_status.loading_depress_vent_valve_open = sensor_module.get_loading_depress_vent_valve_open();
  system_status.hydraulic_umbrilical_connected = sensor_module.get_hydraulic_umbrilical_connected();
  system_status.igniter_continuity_ok = sensor_module.get_igniter_continuity_ok();
  system_status.wind_kt = weather_module.get_wind_kt();
}
