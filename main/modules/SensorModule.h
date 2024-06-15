
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

#include <Wire.h>
#include <SPI.h>

#define LOADING_LINE_PRESSURE_PIN 25 // Analógico, medimos voltaje y hay relación lineal con la presión
#define GROUND_PRESURE_PIN 26 // No está en uso, disponible. Analógico, medimos voltaje y hay relación lineal con la presión
#define GROUND_TEMPERATURE_SPI_SCK 4
#define GROUND_TEMPERATURE_SPI_CS 2
#define GROUND_TEMPERATURE_SPI_S0 15
#define UMBRILICAL_CONNECTED_PIN 34
#define UMBRILICAL_FINISHED_DISCONNECT_PIN 35

#define UMBRILICAL_VOLTAGE_TOLERANCE_EPSILON 0.3

#define REFERENCE_VOLTAGE 3.3
#define MAX_PRESSURE_VALUE_BAR 250

class SensorModule {

  SimulatedValveStatus& simulated_valve_status;
  public:

  SensorModule(SimulatedValveStatus& simulated_valve_status_val) : simulated_valve_status(simulated_valve_status_val) {}

  void init() {
    Logger::log("Initializing sensor Module...");
    pinMode(GROUND_TEMPERATURE_SPI_CS, OUTPUT);
    pinMode(GROUND_TEMPERATURE_SPI_S0, INPUT);
    pinMode(GROUND_TEMPERATURE_SPI_SCK, OUTPUT);
    Logger::log("Sensor Module initialized.");
  }

  int get_loading_line_pressure_bar(){
    return (analogRead(LOADING_LINE_PRESSURE_PIN) / 4095.0) * MAX_PRESSURE_VALUE_BAR;
  }

  int get_ground_temperature_celsius() {
    uint16_t v;
    digitalWrite(GROUND_TEMPERATURE_SPI_CS, LOW);
    delay(1);

    // Read in 16 bits,
    //  15    = 0 always
    //  14..2 = 0.25 degree counts MSB First
    //  2     = 1 if thermocouple is open circuit
    //  1..0  = uninteresting status

    v = shiftIn(GROUND_TEMPERATURE_SPI_S0, GROUND_TEMPERATURE_SPI_SCK, MSBFIRST);
    v <<= 8;
    v |= shiftIn(GROUND_TEMPERATURE_SPI_S0, GROUND_TEMPERATURE_SPI_SCK, MSBFIRST);

    digitalWrite(GROUND_TEMPERATURE_SPI_CS, HIGH);
    if (v & 0x4)
    {
        // Bit 2 indicates if the thermocouple is disconnected
        return NAN;
    }

    // The lower three bits (0,1,2) are discarded status bits
    v >>= 3;

    // The remaining bits are the number of 0.25 degree (C) counts
    return v * 0.25;
  }

  bool get_obec_connection_ok(){
    return true; // Deprecated
  }

  bool get_loading_valve_open(){
    return simulated_valve_status.loading_valve_open;
  }

  bool get_loading_depress_vent_valve_open(){
    return simulated_valve_status.loading_depress_vent_valve_open;
  }

  bool get_hydraulic_umbrilical_connected() {
    return digitalRead(UMBRILICAL_CONNECTED_PIN) > HIGH - UMBRILICAL_VOLTAGE_TOLERANCE_EPSILON;
  }

    bool get_hydraulic_umbrilical_finished_disconnect() {
    return digitalRead(UMBRILICAL_FINISHED_DISCONNECT_PIN) > HIGH - UMBRILICAL_VOLTAGE_TOLERANCE_EPSILON;
  }

  bool get_igniter_continuity_ok(){
    return true; // TODO: Implement
  }

};
