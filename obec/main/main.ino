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

void update_sensor_data() {
  system_status.tank_pressure_psi = sensor_module.get_tank_pressure_psi();
  system_status.tank_temperature_celsius = sensor_module.get_tank_temperature_celsius();
  system_status.tank_depress_vent_temperature_celsius = sensor_module.get_tank_depres_vent_temperature_celsius();
  system_status.obec_battery_voltage_volt = sensor_module.get_obec_battery_voltage_volt();
}
