#include "Logger.h"
#include "ControlModule.h"
#include "ElectromechanicalModule.h"
#include "CommunicationModule.h"
#include "SensorModule.h"
#include "SystemStatus.h"
#include <stdint.h>
#include <string>
#include <format>
#include <vector>


const int MAX_LOADING_LINE_PRESSURE_PSI = 100;
const int MAX_TANK_PRESSURE_PSI = 100;

const float TANK_DEPRESS_VENT_LOW_TEMP = 10;


// System status variables
SystemStatus system_status = SystemStatus();
unsigned long last_milis = 0;

// Modules
StorageModule storage_module = StorageModule();
ControlModule control_module = ControlModule();
SensorModule sensor_module = SensorModule();
CommunicationModule communication_module = CommunicationModule();

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
      
      lastMilis = currMillis;
    }
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
  case LOADING:
    if (system_status.loading_line_pressure_psi < MAX_LOADING_LINE_PRESSURE_PSI &&
          system_status.tank_pressure_psi < MAX_TANK_PRESSURE_PSI){
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
    preflight_check_data.lc_battery_voltage_volt = system_status.lc_battery_voltage_volt;
    preflight_check_data.flight_computers_status = new FlightComputerStatus(system_status.)
    


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