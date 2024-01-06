#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H
#include "ControlModule.h"
#include "CommunicationModule.h"
#include "SensorModule.h"
#include <stdint.h>
#include <string.h>


// System status variables
OBECStatus system_status = OBECStatus();
unsigned long last_milis = 0;
unsigned long last_lc_update_milis = 0;

// Modules
ControlModule control_module = ControlModule();
SensorModule sensor_module = SensorModule();
CommunicationModule communication_module = CommunicationModule();

void setup()
{
  Serial.begin(19200);

  sensor_module.init();
  control_module.init();
  communication_module.init();

  last_milis = millis();
}

void loop()
{
  unsigned long currMilis = millis();

  if (currMilis - last_lc_update_milis > 300) {
    communication_module.check_for_LC_commands();
  
    if (communication_module.new_LC_command_available) {
      control_module.execute_valve_command(communication_module.latest_LC_command);
      communication_module.new_LC_command_available = false;
    }

    update_sensor_data();

    communication_module.send_system_status_to_LC(system_status);

    last_lc_update_milis = currMilis;
  }

}

// Format is <tank_p><tank_t><tank_depress_vent_t><obec_voltage><sensor_data_byte>
// Example result (minus the spaces): 0014 10.1 25.2 4.12 ?
String create_OBEC_status_packet_str(OBECStatus status)
{
  char buffer[10];
  Logger::debug("Creating telemetry packet...");
  String state_str = get_state_string(status.current_state);
  sprintf(buffer, "%0*d", 4, status.tank_pressure_psi); // 4 is the desired number of digits (using leading zeroes)
  String tank_pressure_str(buffer);
  sprintf(buffer, "%.1f", status.tank_temperature_celsius);
  String tank_temperature_str(buffer);
  sprintf(buffer, "%.1f", status.tank_depress_vent_temperature_celsius);
  String tank_depress_vent_temperature_str(buffer);
  sprintf(buffer, "%.12f", status.obec_battery_voltage_volt);
  String obec_battery_voltage_str(buffer);
  String status_flags_str = String(create_status_flags_byte(status.tank_depress_vent_valve_open,
                                                            status.engine_valve_open), 1);

  sprintf(buffer, "%0*d", 3, status.wind_kt); // 3 is the desired number of digits (using leading zeroes)
  String wind_str(buffer);

  return tank_pressure_str +
          tank_temperature_str +
          tank_depress_vent_temperarture_str +
          obec_battery_voltage_str +
          status_flags_str;
}

char create_status_flags_byte(bool tank_depress_vent_valve_open,
                              bool engine_valve_open) {
    char result = 0;
    if (tank_depress_vent_valve_open) result++;
    result<<1;
    if (engine_valve_open) result++;
    return result;
}

String get_state_string(State state) {
  return "STBY"; //TODO: Implement
}

void update_sensor_data() {
  system_status.tank_pressure_psi = sensor_module.get_tank_pressure_psi();
  system_status.tank_temperature_celsius = sensor_module.get_tank_temperature_celsius();
  system_status.tank_depress_vent_temperature_celsius = sensor_module.get_tank_depres_vent_temperature_celsius();
  system_status.obec_battery_voltage_volt = sensor_module.get_obec_battery_voltage_volt();
}
