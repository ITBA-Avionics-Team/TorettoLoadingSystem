
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

#define LOADING_LINE_PRESSURE_PIN 25 // Analógico, medimos voltaje y hay relación lineal con la presión
#define TANK_GROUND_PRESURE_PIN 26 // Analógico, medimos voltaje y hay relación lineal con la presión
#define TANK_TEMPERATURE_SPI_SCK 23
#define TANK_TEMPERATURE_SPI_CS 21
#define TANK_TEMPERATURE_SPI_S0 19

class SensorModule {
  public:

  void init() {
    Logger::log("Initializing sensor Module...");
  
    Logger::log("Sensor Module initialized.");
  }

  int get_loading_line_pressure_psi(){
    // 0 a 250 bar
    return 0;
  }

  int get_ground_tank_presure_psi(){
    return 0;
  }

  int get_tank_temperature_celsius() {
    return 0;
  }

  float get_lc_battery_voltage_volt(){
    return 0; // Deprecated
  }

  bool get_obec_connection_ok(){
    return true; // Deprecated
  }

  bool get_loading_valve_open(){
    return true; // Deprecated
  }

  bool get_loading_depress_vent_valve_open(){
    return true; // Deprecated
  }

  bool get_hydraulic_umbrilical_connected() {
    return true; //Deprecated
  }

  bool get_igniter_continuity_ok(){
    return true; // TODO: Implement
  }

};
