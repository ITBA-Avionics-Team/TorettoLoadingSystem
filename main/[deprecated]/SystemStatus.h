// enum State
// {
//   STANDBY,
//   PRESSURE_WARNING,
//   PRESSURE_WARNING_EXT_VENT,
//   LOADING,
//   PRE_FLIGHT_CHECK,
//   PRE_LAUNCH_WIND_CHECK,
//   PRE_LAUNCH_UMBRILICAL_DISCONNECT,
//   IGNITION_IGNITERS_ON,
//   IGNITION_OPEN_VALVE,
//   IGNITION_IGNITERS_OFF,
//   ABORT
// };

// class SystemStatus {
//   public:
//   State currentState;
//   int tank_pressure_bar;
//   float tank_temperature_celsius;
//   float tank_depress_vent_temperature_celsius;
//   int loading_line_pressure_bar;
//   float obec_battery_voltage_volt;
//   float lc_battery_voltage_volt;
//   bool obec_connection_ok;
//   bool tank_depress_vent_valve_open;
//   bool engine_valve_open;
//   bool loading_valve_open;
//   bool loading_depress_vent_valve_open;
//   bool hydraulic_umbrilical_connected;
//   bool igniter_continuity_ok;
//   bool external_vent_as_default;
//   int wind_kt;
// };

// class OBECStatus {
//   public:
//   int tank_pressure_bar;
//   float tank_temperature_celsius;
//   float tank_depress_vent_temperature_celsius;
//   float obec_battery_voltage_volt;
//   bool tank_depress_vent_valve_open;
//   bool engine_valve_open;
// };
