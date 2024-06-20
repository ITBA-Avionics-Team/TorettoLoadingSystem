
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

#include <Wire.h>
#include <SPI.h>

#define LOADING_LINE_PRESSURE_PIN 25 // Analógico, medimos voltaje y hay relación lineal con la presión
#define GROUND_PRESSURE_PIN 26 // Analógico, medimos corriente
#define GROUND_TEMPERATURE_SPI_SCK 4
#define GROUND_TEMPERATURE_SPI_CS 2
#define GROUND_TEMPERATURE_SPI_S0 15
#define UMBRILICAL_CONNECTED_PIN 34
#define UMBRILICAL_FINISHED_DISCONNECT_PIN 35

#define UMBRILICAL_VOLTAGE_TOLERANCE_EPSILON 0.3

#define REFERENCE_VOLTAGE 3.3
#define MAX_PRESSURE_VALUE_BAR 250.0
#define MAX_GROUND_PRESSURE_VALUE_BAR 60.0

class SensorModule {

  SimulatedValveStatus& simulated_valve_status;
  public:

  SensorModule(SimulatedValveStatus& simulated_valve_status_val) : simulated_valve_status(simulated_valve_status_val) {}

  void init() {
    Logger::log("Initializing sensor Module...");
    pinMode(GROUND_TEMPERATURE_SPI_CS, OUTPUT);
    pinMode(GROUND_TEMPERATURE_SPI_S0, INPUT);
    pinMode(GROUND_TEMPERATURE_SPI_SCK, OUTPUT);
    pinMode(UMBRILICAL_CONNECTED_PIN, INPUT_PULLUP);
    pinMode(UMBRILICAL_FINISHED_DISCONNECT_PIN, INPUT_PULLUP);
    pinMode(GROUND_PRESSURE_PIN, INPUT);
    pinMode(LOADING_LINE_PRESSURE_PIN, INPUT);
    Logger::log("Sensor Module initialized.");
  }

  int get_loading_line_pressure_bar(){ // Presion en base a la tension
  int readResult = analogRead(LOADING_LINE_PRESSURE_PIN);
  Serial.println(String(readResult));
  double result = (readResult / 4095.0) * MAX_PRESSURE_VALUE_BAR;
  double adjusted_result = adjust_loading_line_pressure_value(result);
  // Serial.println(String(readResult));
  // Serial.println(String(result));
    return round(adjusted_result);
  }

   double adjust_loading_line_pressure_value(double value)
  {
    return 1.5 * value;
  }

  int get_ground_pressure_bar(){ // Presion en base a la corriente
  int readResult = analogRead(GROUND_PRESSURE_PIN);
  double result = (( readResult/ 4095.0) * 4.85) * 38.125 - 16.25; // 0.4v to 2V
  double adjusted_result = adjust_ground_pressure_value(result);
  // Serial.println(String(readResult));
  // Serial.println(String(result));
  return round(adjusted_result);
  }

    double adjust_ground_pressure_value(double value)
  {
    return 0.7317073 * value;
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
        return -17;
    }

    // The lower three bits (0,1,2) are discarded status bits
    v >>= 3;

    // The remaining bits are the number of 0.25 degree (C) counts
    return v * 0.25;
  }

  bool get_engine_valve_open(){
    return simulated_valve_status.engine_valve_open;
  }

  bool get_loading_valve_open(){
    return simulated_valve_status.loading_valve_open;
  }

  bool get_hydraulic_umbrilical_connected() {
    // Serial.println(String(analogRead(UMBRILICAL_CONNECTED_PIN)));
    return digitalRead(UMBRILICAL_CONNECTED_PIN) == 0;
  }

    bool get_hydraulic_umbrilical_finished_disconnect() {
    // Serial.println(String(analogRead(UMBRILICAL_FINISHED_DISCONNECT_PIN)));
    return digitalRead(UMBRILICAL_FINISHED_DISCONNECT_PIN) == 0;
  }

  bool get_igniter_continuity_ok(){
    return true; // TODO: Implement
  }

};
