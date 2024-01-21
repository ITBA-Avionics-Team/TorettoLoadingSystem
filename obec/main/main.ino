#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "TorettoLib.h"
#endif // TORETTOLIB_H

#define SIMULATION_MODULE true
#define SIMULATED_SENSOR_MODULE true
// #define SIMULATED_COMMUNICATION_MODULE true

#include <stdint.h>
#include <string.h>


// System status variables
OBECStatus system_status = OBECStatus();
unsigned long last_milis = 0;
unsigned long last_lc_update_milis = 0;
unsigned long simulation_mode_update_millis = 0;

// Modules
#ifdef SIMULATION_MODULE
#include "simulation_modules/SimulationModule.h"
SimulationModule simulation_module = SimulationModule();
#endif


#include "modules/ControlModule.h"
ControlModule control_module = ControlModule();

#ifdef SIMULATED_SENSOR_MODULE
#include "simulation_modules/SimulatedSensorModule.h"
SimulatedSensorModule sensor_module = SimulatedSensorModule(simulation_module);
#else
#include "modules/SensorModule.h"
SensorModule sensor_module = SensorModule();
#endif


#ifdef SIMULATED_COMMUNICATION_MODULE
#include "simulation_modules/SimulatedCommunicationModule.h"
SimulatedCommunicationModule communication_module = SimulatedCommunicationModule(simulation_module);
#else
#include "modules/CommunicationModule.h"
CommunicationModule communication_module = CommunicationModule();
#endif

void setup()
{
  Serial.begin(115200);

  sensor_module.init();
  control_module.init();
  communication_module.init();

  last_milis = millis();
}

void loop()
{
  unsigned long currMilis = millis();

#ifdef SIMULATION_MODULE
  if (currMilis - simulation_mode_update_millis > 200) {
    simulation_module.loop();  
    simulation_mode_update_millis = currMilis;
  }
#endif

  if (currMilis - last_lc_update_milis > 300) {
    // communication_module.check_for_LC_commands();
  
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
