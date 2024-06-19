
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

#define TANK_DEPRESSS_VENT_TEMPERATURE_SPI_SCK 15
#define TANK_DEPRESSS_VENT_TEMPERATURE_SPI_CS 2
#define TANK_DEPRESSS_VENT_TEMPERATURE_SPI_S0 4

#define BATTERY_VOLTAGE_PIN 32

class SensorModule
{
public:
  
  void init()
  {
    Logger::log("Initializing sensor Module...");
    pinMode(TANK_DEPRESSS_VENT_TEMPERATURE_SPI_CS, OUTPUT);
    pinMode(TANK_DEPRESSS_VENT_TEMPERATURE_SPI_S0, INPUT);
    pinMode(TANK_DEPRESSS_VENT_TEMPERATURE_SPI_SCK, OUTPUT);
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    Logger::log("Sensor Module initialized.");
  }

  float get_tank_depres_vent_temperature_celsius()
  {
    uint16_t v;
    digitalWrite(TANK_DEPRESSS_VENT_TEMPERATURE_SPI_CS, LOW);
    delay(1);

    // Read in 16 bits,
    //  15    = 0 always
    //  14..2 = 0.25 degree counts MSB First
    //  2     = 1 if thermocouple is open circuit
    //  1..0  = uninteresting status

    v = shiftIn(TANK_DEPRESSS_VENT_TEMPERATURE_SPI_S0, TANK_DEPRESSS_VENT_TEMPERATURE_SPI_SCK, MSBFIRST);
    v <<= 8;
    v |= shiftIn(TANK_DEPRESSS_VENT_TEMPERATURE_SPI_S0, TANK_DEPRESSS_VENT_TEMPERATURE_SPI_SCK, MSBFIRST);

    digitalWrite(TANK_DEPRESSS_VENT_TEMPERATURE_SPI_CS, HIGH);
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

  float get_obec_battery_voltage_volt()
  {
    // Logger::log(analogRead(BATTERY_VOLTAGE_PIN));
    return (analogRead(BATTERY_VOLTAGE_PIN) / 4095.0) * 8.4;
  }
};
