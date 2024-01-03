// SystemStatus.h
enum State
{
  STANDBY,
  STANDBY_PRESSURE_WARNING,
  STANDBY_PRESSURE_WARNING_EXTERNAL_VENT,
  LOADING,
  PRE_FLIGHT_CHECK,
  PRE_LAUNCH_WIND_CHECK,
  PRE_LAUNCH_UMBRILICAL_DISCONNECT,
  IGNITION_IGNITERS_ON,
  IGNITION_OPEN_VALVE,
  IGNITION_IGNITERS_OFF,
  ABORT
};

class SystemStatus {
  public:
  State current_state;
  int tank_pressure_psi;
  float tank_temperature_celsius;
  float tank_depress_vent_temperature_celsius;
  int loading_line_pressure_psi;
  float obec_battery_voltage_volt;
  float lc_battery_voltage_volt;
  bool obec_connection_ok;
  bool tank_depress_vent_valve_open;
  bool engine_valve_open;
  bool loading_valve_open;
  bool loading_depress_vent_valve_open;
  bool hydraulic_umbrilical_connected;
  bool igniter_continuity_ok;
  bool external_vent_as_default;
  int wind_kt;
};

class OBECStatus {
  public:
  int tank_pressure_psi;
  float tank_temperature_celsius;
  float tank_depress_vent_temperature_celsius;
  float obec_battery_voltage_volt;
  bool tank_depress_vent_valve_open;
  bool engine_valve_open;
};


// ValveControl.h
enum Valve {
  TANK_DEPRESS_VENT_VALVE,
  ENGINE_VALVE,
  LOADING_VALVE,
  LOADING_LINE_DEPRESS_VENT_VALVE
};

enum ValveAction {
  Open,
  Close
};

class ValveCommand {
  public:
    ValveAction action;
    Valve valve;

    ValveCommand(ValveAction actionVal, Valve valveVal) {
      action = actionVal;
      valve = valveVal;
    }
};


// PreflightCheck.h
class FlightComputersStatus {
  public:
  bool main_ready;
  bool backup_ready;
  bool srad_ready;
  FlightComputersStatus(): main_ready(false), backup_ready(false), srad_ready(false) {};
  FlightComputersStatus(bool main_ready_val, bool backup_ready_val, bool srad_ready_val) {
    main_ready = main_ready_val;
    backup_ready = backup_ready_val;
    srad_ready = srad_ready_val;
  };
};

class WeatherData {
  public:
  int wind_kt;
  WeatherData(int wind_kt_val) {
    wind_kt = wind_kt_val;
  };
  WeatherData(): wind_kt(0) {};
};

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

