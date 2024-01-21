enum Module
{
  COMMUNICATION_MODULE,
  SENSOR_MODULE,
  WEATHER_MODULE,
  STORAGE_MODULE
};

String get_2_byte_string_from_module(Module module)
{
  switch (module)
  {
  case COMMUNICATION_MODULE:
    return "CM";
  case SENSOR_MODULE:
    return "SE";
  case WEATHER_MODULE:
    return "WE";
  case STORAGE_MODULE:
    return "ST";
  }
}

Module get_module_from_2_byte_string(String module_str)
{

  if (module_str.equals("CM")) {
    return COMMUNICATION_MODULE;
  } 
  else if (module_str.equals("SE")) {
    return SENSOR_MODULE;
  } 
  else if (module_str.equals("WE")) {
    return WEATHER_MODULE;
  } 
  else if (module_str.equals("ST")) {
    return STORAGE_MODULE;
  } 
}

// Return type should be associated with the module+function ID
// enum ReturnType
// {
//   COMMAND,
//   OBEC_STATUS,
//   BOOL,
//   FLOAT
// }

class SimulationCommand
{
public:
  Module module;
  int function_id;
  String value;

  SimulationCommand(Module module_val, int function_id_val, String value_val)
  {
    module = module_val;
    function_id = function_id_val;
    value = value_val;
  };

  static SimulationCommand from_message(String message)
  {

    Module module_id = get_module_from_2_byte_string(message.substring(0, 2));

    int function_id = message.substring(3, 4).toInt();
    
    String value = message.substring(5, message.length());

    return SimulationCommand(module_id, function_id, value);
  }
};
