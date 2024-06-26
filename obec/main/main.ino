#define LED_PIN 25
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

#ifndef TORETTOLIB_H
#define TORETTOLIB_H

#include "TorettoLib.h"
#endif // TORETTOLIB_H

// #define SIMULATION_MODULE true
// #define SIMULATED_SENSOR_MODULE true
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
  Serial.begin(115200, SERIAL_8N1, 3, 1);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  pinMode(RS485_SET_TX_PIN, OUTPUT);
  pinMode(RS485_SET_RX_PIN, OUTPUT);
  digitalWrite(RS485_SET_TX_PIN, HIGH); // We set ourselves as the transamitter
  digitalWrite(RS485_SET_RX_PIN, HIGH); // We set ourselves as the transamitter

  sensor_module.init();
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

  if (currMilis - last_lc_update_milis > 500) {
    update_sensor_data();
    communication_module.send_system_status_to_LC(system_status);
    last_lc_update_milis = currMilis;
  }
}

void update_sensor_data() {
  system_status.tank_depress_vent_temperature_celsius = sensor_module.get_tank_depres_vent_temperature_celsius();
  system_status.obec_battery_voltage_volt = sensor_module.get_obec_battery_voltage_volt();
}
