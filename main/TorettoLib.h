#define Close 0
#define Open 255

// SystemStatus.h
enum State
{
  STANDBY,
  STANDBY_PRESSURE_WARNING,
  LOADING,
  PRE_FLIGHT_CHECK,
  PRE_LAUNCH_WIND_CHECK,
  PRE_LAUNCH_UMBRILICAL_DISCONNECT,
  IGNITION_OPEN_VALVE,
  IGNITION_IGNITERS_ON,
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
  case LOADING:
    return "LDNG";
  case PRE_FLIGHT_CHECK:
    return "PRFC";
  case PRE_LAUNCH_WIND_CHECK:
    return "PRLW";
  case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
    return "PRLU";
  case IGNITION_OPEN_VALVE:
    return "IGVO";
  case IGNITION_IGNITERS_ON:
    return "IGON";
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
  else if (state_str.equals("IGVO")) {
    return IGNITION_OPEN_VALVE;
  } 
  else if (state_str.equals("IGON")) {
    return IGNITION_IGNITERS_ON;
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
  case LOADING:
    return "LOADING";
  case PRE_FLIGHT_CHECK:
    return "PRE_FLIGHT_CHECK";
  case PRE_LAUNCH_WIND_CHECK:
    return "PRE_LAUNCH_WIND_CHECK";
  case PRE_LAUNCH_UMBRILICAL_DISCONNECT:
    return "PRE_LAUNCH_UMBRILICAL_DISCONNECT";
  case IGNITION_OPEN_VALVE:
    return "IGNITION_OPEN_VALVE";
  case IGNITION_IGNITERS_ON:
    return "IGNITION_IGNITERS_ON";
  case IGNITION_IGNITERS_OFF:
    return "IGNITION_IGNITERS_OFF";
  case ABORT:
    return "ABORT";
  }
}

// Examples: 100.0 095.1 005.4 -09.0 -99.5
String get_formatted_temp_string(float temp) {
  if (temp >= 999) return String("999.0");
  if (temp >= 100) return String(temp, 1);
  if (temp >= 10) return String("0") + String(temp, 1);
  if (temp >= 0) return String("00") + String(temp, 1);
  if (temp >= -9) return String("-0") + String(-temp, 1);
  if (temp >= -99) return String(temp, 1);
  return "-99.0";
}

// Examples: 99.17 09.60 -8.45 -23.4
String get_formatted_voltage_string(float voltage) {
  if (voltage >= 10) return String(voltage, 2);
  if (voltage >= 0) return String("0") + String(voltage, 2);
  if (voltage >= -9) return String(voltage, 2);
  if (voltage >= -99) return String(voltage, 1);
  return String("-99.9");
}


class SystemStatus
{

public:
  State current_state;
  float tank_depress_vent_temperature_celsius;
  int loading_line_pressure_bar;
  float ground_temperature_celsius;
  float obec_battery_voltage_volt;
  bool obec_connection_ok;
  bool engine_valve_open;
  bool loading_valve_open;
  bool loading_depress_vent_valve_open;
  bool hydraulic_umbrilical_connected;
  bool hydraulic_umbrilical_finished_disconnect;
  bool igniter_continuity_ok;
  bool external_vent_as_default;
  int wind_kt;

  // Format is <state><tank_depress_vent_t><loading_line_p><ground_t><obec_voltage>
  //           <obec_connection_ok><engine_valve_open><loading_valve_open><loading_depress_vent_valve_open>
  //           <hydraulic_umbrilical_connected><hydraulic_umbrilical_finished_disconnect>
  //           <igniter_continuity_ok><external_vent_as_default><wind_kt>
  // Example result (minus the spaces): STBY 000.0 0000 000.0 00.00 0 0 0 0 0 0 0 0 000
  static String to_message(SystemStatus status)
  {
    char buffer[10];
    Logger::debug("Creating SystemStatus message...");
    String state_str = get_4_byte_string_from_state(status.current_state);
    String tank_depress_vent_temperature_celsius_str = get_formatted_temp_string(status.tank_depress_vent_temperature_celsius);
    sprintf(buffer, "%04d", status.loading_line_pressure_bar); // 4 is the desired number of digits (using leading zeroes)
    String loading_line_pressure_str(buffer);
    String ground_temperature_celsius_str = get_formatted_temp_string(status.ground_temperature_celsius);
    String obec_battery_voltage_str = get_formatted_voltage_string(status.obec_battery_voltage_volt);
    String obec_connection_ok_str = String(status.obec_connection_ok ? "1" : "0");
    String engine_valve_open_str = String(status.engine_valve_open ? "1" : "0");
    String loading_valve_open_str = String(status.loading_valve_open ? "1" : "0");
    String loading_depress_vent_valve_open_str = String(status.loading_depress_vent_valve_open ? "1" : "0");
    String hydraulic_umbrilical_connected_str  = String(status.hydraulic_umbrilical_connected ? "1" : "0");
    String hydraulic_umbrilical_finished_disconnect_str  = String(status.hydraulic_umbrilical_finished_disconnect ? "1" : "0");
    String igniter_continuity_ok_str = String(status.igniter_continuity_ok ? "1" : "0");
    String external_vent_as_default_str = String(status.external_vent_as_default ? "1" : "0");
    sprintf(buffer, "%03d", status.wind_kt); // 3 is the desired number of digits (using leading zeroes)
    String wind_str(buffer);

    return state_str +
          tank_depress_vent_temperature_celsius_str +
          loading_line_pressure_str +
          ground_temperature_celsius_str +
          obec_battery_voltage_str +
          obec_connection_ok_str +
          engine_valve_open_str +
          loading_valve_open_str +
          loading_depress_vent_valve_open_str +
          hydraulic_umbrilical_connected_str +
          hydraulic_umbrilical_finished_disconnect_str +
          igniter_continuity_ok_str +
          external_vent_as_default_str +
          wind_str;
  }
};

class OBECStatus
{

public:
  float tank_depress_vent_temperature_celsius = -1;
  float obec_battery_voltage_volt = -1;
  bool engine_valve_open = false;

  OBECStatus() {}

  OBECStatus(float tank_depress_vent_temperature_celsius_val,
             float obec_battery_voltage_volt_val,
             bool engine_valve_open_val)
  {
    tank_depress_vent_temperature_celsius = tank_depress_vent_temperature_celsius_val;
    obec_battery_voltage_volt = obec_battery_voltage_volt_val;
    engine_valve_open = engine_valve_open_val;
  }

  // Format is <tank_depress_vent_t><obec_voltage><engine_valve_open>
  // Example result (minus the spaces): -17.0 00.00 0|
  static String to_message(OBECStatus status)
  {
    char buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Logger::debug("Creating OBECStatus message...");
    String tank_depress_vent_temperature_str = get_formatted_temp_string(status.tank_depress_vent_temperature_celsius);
    String obec_battery_voltage_str = get_formatted_voltage_string(status.obec_battery_voltage_volt);
    String engine_valve_open_str = String(status.engine_valve_open ? "1" : "0");

    return tank_depress_vent_temperature_str +
           obec_battery_voltage_str +
           engine_valve_open_str;
  }

  static OBECStatus from_message(String message)
  {
    // TODO: if invalid, should return empty status
    OBECStatus result = OBECStatus(
        message.substring(0, 5).toFloat(),
        message.substring(5, 10).toFloat(),
        message.charAt(10) == '1'
      );
    return result;
  }
  static bool is_empty(OBECStatus status)
  { // TODO: We should create some king of static OBECStatus::EMPTY constant
    return status.tank_depress_vent_temperature_celsius == -1 &&
           status.obec_battery_voltage_volt == -1 &&
           status.engine_valve_open == false;
  }
};

// ValveControl.h
enum Valve
{
  ENGINE_VALVE,
  LOADING_VALVE,
};

bool is_LC_valve(Valve valve)
{
  return (valve == LOADING_VALVE);
}

String get_4_byte_string_from_valve(Valve valve)
{
  switch (valve)
  {
  case ENGINE_VALVE:
    return "ENGV";
  case LOADING_VALVE:
    return "LDGV";
  }
}

Valve get_valve_from_4_byte_string(String valve_str)
{
  if (valve_str.equals("ENGV")) {
    return ENGINE_VALVE;
  } 
  else if (valve_str.equals("LDGV")) {
    return LOADING_VALVE;
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
          case ENGINE_VALVE:
            return String("VCENGV") + (command.uint_value == Open ? "1" : "0") + String("|");
            break;
          case LOADING_VALVE:
            return String("VCLDGV") + (command.uint_value == Open ? "1" : "0") + String("|");
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
      return Command(ValveCommand, message.charAt(6) == '0' ? Close : Open, get_valve_from_4_byte_string(message.substring(2, 6)));
    }
    else if (command_type_str.equals("EV"))
    {
      return Command(SetExternalVentAsDefaultCommand, message.charAt(2) == '1');
    } else {
      return Command(EMPTY);
    }
  }

  static bool is_empty(Command comm)
  {
    return comm.type == EMPTY;
  }
};

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
  float loading_line_pressure_bar;
  bool engine_valve_open;
  bool loading_valve_open;
  float obec_battery_voltage_volt;
  FlightComputersStatus flight_computers_status;
  bool igniter_continuity_ok;
  WeatherData weather_data;
};

class SimulatedValveStatus {
  public:
  bool loading_valve_open = false;
  bool loading_depress_vent_valve_open = false;
  bool tank_depress_vent_valve_open = false;
  bool engine_valve_open = false;
};
