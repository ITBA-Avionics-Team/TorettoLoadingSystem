#include <string>
class PreflightCheckData {

  public:
  float tank_pressure_psi;
  float tank_temperature_celsius;
  bool engine_valve_open;
  bool loading_valve_open;
  float obec_battery_voltage_volt;
  float lc_battery_voltage_volt;
  FlightComputersStatus flight_computers_status;
  bool igniter_continuity_ok;
  WeatherData weather_data;
};

class FlightComputersStatus {
  public:
  bool main_ready;
  bool backup_ready;
  bool srad_ready;
  FlightComputersStatus(bool main_ready_val, bool backup_ready_val, bool srad_ready_val) {
    main_ready = main_ready_val;
    backup_ready = backup_ready_val;
    srad_ready = srad_ready_val;
  }
};

class WeatherData {
  public:
  int wind_kt;
  WeatherData(int wind_kt_val) {
    wind_kt = wind_kt_val;
  }
};
