#define Close 0
#define Open 255

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

String get_state_4_byte_string(State state) {
  switch (state)
  {
  case STANDBY:
    return "STBY";
  case STANDBY_PRESSURE_WARNING:
    return "STPW";
  case STANDBY_PRESSURE_WARNING_EXTERNAL_VENT:
    return "STPE";
  case LOADING:
    return "LDNG";
  case PRE_FLIGHT_CHECK:
    return "PRFC";
  case PRE_LAUNCH_WIND_CHECK:
    return "PRLW";
  case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
    return "PRLU";
  case IGNITION_IGNITERS_ON:
    return "IGON";
  case IGNITION_OPEN_VALVE:
    return "IGVO";
  case IGNITION_IGNITERS_OFF:
    return "IGOF";
  case ABORT:
    return "ABRT";
  }
}

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

  OBECStatus(){}

  OBECStatus(int tank_pressure_psi_val,
              float tank_temperature_celsius_val,
              float tank_depress_vent_temperature_celsius_val,
              float obec_battery_voltage_volt_val,
              bool tank_depress_vent_valve_open_val,
              bool engine_valve_open_val){
    tank_pressure_psi = tank_pressure_psi_val;
    tank_temperature_celsius = tank_temperature_celsius_val;
    tank_depress_vent_temperature_celsius = tank_depress_vent_temperature_celsius_val;
    obec_battery_voltage_volt = obec_battery_voltage_volt_val;
    tank_depress_vent_valve_open = tank_depress_vent_valve_open_val;
    engine_valve_open = engine_valve_open_val;
  }

  static OBECStatus from_message(String message) {
    char sensor_data_byte = message.charAt(16);
    OBECStatus result = OBECStatus(
      message.substring(0,4).toInt(),
      message.substring(4, 8).toFloat(),
      message.substring(8, 12).toFloat(),
      message.substring(12, 16).toFloat(),
      sensor_data_byte & 0b01,
      sensor_data_byte & 0b10
    );
    return result;
  }
  static bool is_empty(OBECStatus status) { // TODO: We should create some king of static OBECStatus::EMPTY constant
    return tank_pressure_psi == -1 &&
            tank_temperature_celsius == -1 &&
            tank_depress_vent_temperature_celsius == -1 &&
            obec_battery_voltage_volt == -1 &&
            tank_depress_vent_valve_open == false &&
            engine_valve_open == false;
  }
};

// ValveControl.h
enum Valve {
  TANK_DEPRESS_VENT_VALVE,
  ENGINE_VALVE,
  LOADING_VALVE,
  LOADING_LINE_DEPRESS_VENT_VALVE
};

bool is_LC_valve(Valve valve) {
  return (valve == LOADING_VALVE || valve == LOADING_LINE_DEPRESS_VENT_VALVE);
}

// enum ValveAction {
//   Open,
//   Close
// };

enum CommandType {
  ValveCommand,
  SwitchStateCommand,
  SetExternalVentAsDefaultCommand,
  EMPTY
};

class Command {
  public:
  CommandType type;
  char uint_value;
  Valve valve;
  State state;
  bool bool_value;
  Command(CommandType type_val, char uint_value_val, Valve valve_val) {
    type = type_val;
    valve = valve_val;
    uint_value = uint_value_val;
  };
  Command(CommandType type_val, State state_val) {
    type = type_val;
    state = state_val;
  };
  Command(CommandType type_val, bool bool_value_val) {
    type = type_val;
    bool_value = bool_value_val;
  };
  Command(CommandType type_val) {
    type = type_val;
  };
  Command(){}

  static String to_message(Command command) {
    switch (command.type) {
      case ValveCommand:
        switch (command.valve) {
          case TANK_DEPRESS_VENT_VALVE:
            return "VC" + "TDVV" + String((char)command.int_value, 1) + "|";
            break;
          case ENGINE_VALVE:
            return "VC" + "ENGV" + String((char)command.int_value, 1) + "|";
            break;
          case LOADING_VALVE:
            return "VC" + "LDGV" + String((char)command.int_value, 1) + "|";
            break;
          case LOADING_LINE_DEPRESS_VENT_VALVE:
            return "VC" + "LDVV" + String((char)command.int_value, 1) + "|";
            break;
        }
        break;
      case SwitchStateCommand:
        return "SS" + get_state_4_byte_string(command.state) + "|";
        break;
      case SetExternalVentAsDefaultCommand:
        return "EV" + command.bool_value ? "1" : "0" + "|";
        break;
      case EMPTY:
        break;
    }
  }

  static bool is_empty(Command comm) {
    return comm.type == EMPTY;
  }
};

// class ValveCommand: Command {
//   public:
//     ValveAction action;
//     Valve valve;

//     ValveCommand(ValveAction actionVal, Valve valveVal) {
//       action = actionVal;
//       valve = valveVal;
//     }
// };

// class SwitchStateCommand: Command {
//   State state;
//   SwitchStateCommand(State val) {
//     state = val;
//   }
// }

// class SetExternalVentAsDefaultCommand: Command {
//   bool ext_vent_as_default;
//   SetExternalVentAsDefaultCommand(bool val) {
//     ext_vent_as_default = val;
//   }
// }


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

