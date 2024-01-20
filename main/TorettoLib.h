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

String get_4_byte_string_from_state(State state)
{
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

State get_state_from_4_byte_string(String state_str)
{

  if (state_str.equals("STBY")) {
    return STANDBY;
  } 
  else if (state_str.equals("STPW")) {
    return STANDBY_PRESSURE_WARNING;
  } 
  else if (state_str.equals("STPE")) {
    return STANDBY_PRESSURE_WARNING_EXTERNAL_VENT;
  } 
  else if (state_str.equals("LDNG")) {
    return LOADING;
  } 
  else if (state_str.equals("PRFC")) {
    return PRE_FLIGHT_CHECK;
  } 
  else if (state_str.equals("PRLW")) {
    return PRE_LAUNCH_WIND_CHECK;
  } 
  else if (state_str.equals("PRLU")) {
    return PRE_LAUNCH_UMBRILICAL_DISCONNECT;
  } 
  else if (state_str.equals("IGON")) {
    return IGNITION_IGNITERS_ON;
  } 
  else if (state_str.equals("IGVO")) {
    return IGNITION_OPEN_VALVE;
  } 
  else if (state_str.equals("IGOF")) {
    return IGNITION_IGNITERS_OFF;
  } 
  else if (state_str.equals("ABRT")) {
    return ABORT;
  }
}

String get_state_complete_string(State state)
{

  switch (state)
  {
  case STANDBY:
    return "STANDBY";
  case STANDBY_PRESSURE_WARNING:
    return "STANDBY_PRESSURE_WARNING";
  case STANDBY_PRESSURE_WARNING_EXTERNAL_VENT:
    return "STANDBY_PRESSURE_WARNING_EXTERNAL_VENT";
  case LOADING:
    return "LOADING";
  case PRE_FLIGHT_CHECK:
    return "PRE_FLIGHT_CHECK";
  case PRE_LAUNCH_WIND_CHECK:
    return "PRE_LAUNCH_WIND_CHECK";
  case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
    return "PRE_LAUNCH_UMBRILICAL_DISCONNECT";
  case IGNITION_IGNITERS_ON:
    return "IGNITION_IGNITERS_ON";
  case IGNITION_OPEN_VALVE:
    return "IGNITION_OPEN_VALVE";
  case IGNITION_IGNITERS_OFF:
    return "IGNITION_IGNITERS_OFF";
  case ABORT:
    return "ABORT";
  }
}

class SystemStatus
{
  static char create_status_flags_byte(bool obec_connection_ok,
                                       bool tank_depress_vent_valve_open,
                                       bool engine_valve_open,
                                       bool loading_valve_open,
                                       bool loading_depress_vent_valve_open,
                                       bool hydraulic_umbrilical_connected,
                                       bool igniter_continuity_ok)
  {
    char result = 0;
    if (obec_connection_ok)
      result++;
    result << 1;
    if (tank_depress_vent_valve_open)
      result++;
    result << 1;
    if (engine_valve_open)
      result++;
    result << 1;
    if (loading_valve_open)
      result++;
    result << 1;
    if (loading_depress_vent_valve_open)
      result++;
    result << 1;
    if (hydraulic_umbrilical_connected)
      result++;
    result << 1;
    if (igniter_continuity_ok)
      result++;
    return result;
  }

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

  // Format is <state><tank_p><tank_t><tank_depress_vent_t><loading_line_p><obec_voltage><lc_voltage><sensor_data_byte><wind>
  // Example result (minus the spaces): STBY 0014 10.1 25.2 0014 4.12 4.20 ? 008
  static String to_message(SystemStatus status)
  {
    char buffer[10];
    Logger::debug("Creating SystemStatus message...");
    String state_str = get_4_byte_string_from_state(status.current_state);
    sprintf(buffer, "%0*d", 4, status.tank_pressure_psi); // 4 is the desired number of digits (using leading zeroes)
    String tank_pressure_str(buffer);
    sprintf(buffer, "%.1f", status.tank_temperature_celsius);
    String tank_temperature_str(buffer);
    sprintf(buffer, "%.1f", status.tank_depress_vent_temperature_celsius);
    String tank_depress_vent_temperature_celsius_str(buffer);
    sprintf(buffer, "%0*d", 4, status.loading_line_pressure_psi); // 4 is the desired number of digits (using leading zeroes)
    String loading_line_pressure_str(buffer);
    sprintf(buffer, "%.2f", status.obec_battery_voltage_volt);
    String obec_battery_voltage_str(buffer);
    sprintf(buffer, "%.2f", status.lc_battery_voltage_volt);
    String lc_battery_voltage_str(buffer);
    String status_flags_str = String(SystemStatus::create_status_flags_byte(status.obec_connection_ok,
                                                                            status.tank_depress_vent_valve_open,
                                                                            status.engine_valve_open,
                                                                            status.loading_valve_open,
                                                                            status.loading_depress_vent_valve_open,
                                                                            status.hydraulic_umbrilical_connected,
                                                                            status.igniter_continuity_ok),
                                     1);
    sprintf(buffer, "%0*d", 3, status.wind_kt); // 3 is the desired number of digits (using leading zeroes)
    String wind_str(buffer);

    return state_str + tank_pressure_str + tank_temperature_str + tank_depress_vent_temperature_celsius_str + loading_line_pressure_str + obec_battery_voltage_str + lc_battery_voltage_str + status_flags_str + wind_str;
  }
};

class OBECStatus
{

  static char create_status_flags_byte(bool tank_depress_vent_valve_open,
                                       bool engine_valve_open)
  {
    char result = 0;
    if (tank_depress_vent_valve_open)
      result++;
    result << 1;
    if (engine_valve_open)
      result++;
    return result;
  }

public:
  int tank_pressure_psi;
  float tank_temperature_celsius;
  float tank_depress_vent_temperature_celsius;
  float obec_battery_voltage_volt;
  bool tank_depress_vent_valve_open;
  bool engine_valve_open;

  OBECStatus() {}

  OBECStatus(int tank_pressure_psi_val,
             float tank_temperature_celsius_val,
             float tank_depress_vent_temperature_celsius_val,
             float obec_battery_voltage_volt_val,
             bool tank_depress_vent_valve_open_val,
             bool engine_valve_open_val)
  {
    tank_pressure_psi = tank_pressure_psi_val;
    tank_temperature_celsius = tank_temperature_celsius_val;
    tank_depress_vent_temperature_celsius = tank_depress_vent_temperature_celsius_val;
    obec_battery_voltage_volt = obec_battery_voltage_volt_val;
    tank_depress_vent_valve_open = tank_depress_vent_valve_open_val;
    engine_valve_open = engine_valve_open_val;
  }

  // Format is <tank_p><tank_t><tank_depress_vent_t><obec_voltage><sensor_data_byte>
  // Example result (minus the spaces): 0014 10.1 25.2 4.12 ?
  static String to_message(OBECStatus status)
  {
    char buffer[10];
    Logger::debug("Creating OBECStatus message...");
    // String state_str = get_4_byte_string_from_state(status.current_state);
    sprintf(buffer, "%0*d", 4, status.tank_pressure_psi); // 4 is the desired number of digits (using leading zeroes)
    String tank_pressure_str(buffer);
    sprintf(buffer, "%.1f", status.tank_temperature_celsius);
    String tank_temperature_str(buffer);
    sprintf(buffer, "%.1f", status.tank_depress_vent_temperature_celsius);
    String tank_depress_vent_temperature_str(buffer);
    sprintf(buffer, "%.12f", status.obec_battery_voltage_volt);
    String obec_battery_voltage_str(buffer);
    String status_flags_str = String(OBECStatus::create_status_flags_byte(status.tank_depress_vent_valve_open,
                                                                          status.engine_valve_open),
                                     1);

    return tank_pressure_str +
           tank_temperature_str +
           tank_depress_vent_temperature_str +
           obec_battery_voltage_str +
           status_flags_str;
  }

  static OBECStatus from_message(String message)
  {
    char sensor_data_byte = message.charAt(16);
    // TODO: if invalid, should return empty status
    OBECStatus result = OBECStatus(
        message.substring(0, 4).toInt(),
        message.substring(4, 8).toFloat(),
        message.substring(8, 12).toFloat(),
        message.substring(12, 16).toFloat(),
        sensor_data_byte & 0b01,
        sensor_data_byte & 0b10);
    return result;
  }
  static bool is_empty(OBECStatus status)
  { // TODO: We should create some king of static OBECStatus::EMPTY constant
    return status.tank_pressure_psi == -1 &&
           status.tank_temperature_celsius == -1 &&
           status.tank_depress_vent_temperature_celsius == -1 &&
           status.obec_battery_voltage_volt == -1 &&
           status.tank_depress_vent_valve_open == false &&
           status.engine_valve_open == false;
  }
};

// ValveControl.h
enum Valve
{
  TANK_DEPRESS_VENT_VALVE,
  ENGINE_VALVE,
  LOADING_VALVE,
  LOADING_LINE_DEPRESS_VENT_VALVE
};

bool is_LC_valve(Valve valve)
{
  return (valve == LOADING_VALVE || valve == LOADING_LINE_DEPRESS_VENT_VALVE);
}

String get_4_byte_string_from_valve(Valve valve)
{
  switch (valve)
  {
  case TANK_DEPRESS_VENT_VALVE:
    return "TDVV";
  case ENGINE_VALVE:
    return "ENGV";
  case LOADING_VALVE:
    return "LDGV";
  case LOADING_LINE_DEPRESS_VENT_VALVE:
    return "LDVV";
  }
}

Valve get_valve_from_4_byte_string(String valve_str)
{
  if (valve_str.equals("TDVV")) {
    return TANK_DEPRESS_VENT_VALVE;
  } 
  else if (valve_str.equals("ENGV")) {
    return ENGINE_VALVE;
  } 
  else if (valve_str.equals("LDGV")) {
    return LOADING_VALVE;
  } 
  else if (valve_str.equals("LDVV")) {
    return LOADING_LINE_DEPRESS_VENT_VALVE;
  } 
}

// enum ValveAction {
//   Open,
//   Close
// };

enum CommandType
{
  ValveCommand,
  SwitchStateCommand,
  SetExternalVentAsDefaultCommand,
  EMPTY
};

class Command
{
public:
  CommandType type;
  char uint_value;
  Valve valve;
  State state;
  bool bool_value;
  Command(CommandType type_val, char uint_value_val, Valve valve_val)
  {
    type = type_val;
    valve = valve_val;
    uint_value = uint_value_val;
  };
  Command(CommandType type_val, State state_val)
  {
    type = type_val;
    state = state_val;
  };
  Command(CommandType type_val, bool bool_value_val)
  {
    type = type_val;
    bool_value = bool_value_val;
  };
  Command(CommandType type_val)
  {
    type = type_val;
  };
  Command() {}

  static String to_message(Command command)
  {
    switch (command.type)
    {
    case ValveCommand:
      switch (command.valve)
      {
      case TANK_DEPRESS_VENT_VALVE:
        return String("VCTDVV") + String((char)command.uint_value, 1) + String("|");
        break;
      case ENGINE_VALVE:
        return String("VCENGV") + String((char)command.uint_value, 1) + String("|");
        break;
      case LOADING_VALVE:
        return String("VCLDGV") + String((char)command.uint_value, 1) + String("|");
        break;
      case LOADING_LINE_DEPRESS_VENT_VALVE:
        return String("VCLDVV") + String((char)command.uint_value, 1) + String("|");
        break;
      }
      break;
    case SwitchStateCommand:
      return String("SS") + get_4_byte_string_from_state(command.state) + String("|");
      break;
    case SetExternalVentAsDefaultCommand:
      return String("EV") + command.bool_value ? String("1") : String("0") + String("|");
      break;
    case EMPTY:
      break;
    }
  }

  static Command from_message(String message)
  {
    String command_type_str = message.substring(0, 2);
    if (command_type_str.equals("SS"))
    {
      return Command(SwitchStateCommand, get_state_from_4_byte_string(message.substring(2, 6)));
    }
    else if (command_type_str.equals("VC"))
    {
      return Command(ValveCommand, message.charAt(6), get_valve_from_4_byte_string(message.substring(2, 6)));
    }
    else if (command_type_str.equals("EV"))
    {
      return Command(SetExternalVentAsDefaultCommand, message.charAt(2) == '1');
    }
  }

  static bool is_empty(Command comm)
  {
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
class FlightComputersStatus
{
public:
  bool main_ready;
  bool backup_ready;
  bool srad_ready;
  FlightComputersStatus() : main_ready(false), backup_ready(false), srad_ready(false){};
  FlightComputersStatus(bool main_ready_val, bool backup_ready_val, bool srad_ready_val)
  {
    main_ready = main_ready_val;
    backup_ready = backup_ready_val;
    srad_ready = srad_ready_val;
  };
};

class WeatherData
{
public:
  int wind_kt;
  WeatherData(int wind_kt_val)
  {
    wind_kt = wind_kt_val;
  };
  WeatherData() : wind_kt(0){};
};

class PreflightCheckData
{

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
