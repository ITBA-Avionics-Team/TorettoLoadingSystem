
class SimulationModule {

    public:
    String communication_module_return_vals[4] = {"","", "", ""};
    String sensor_module_return_vals[2] = {"",""};
    String weather_module_return_vals[1] = {""};
    String storage_module_return_vals[1] = {""};

    // We should probable include parsing functions as a part of the SimulationModule (as static functions?)

    static Command parse_command(String cmd_str) {
        return Command();
    }

    static OBECStatus parse_OBEC_status(String status_str) {
        return OBECStatus();
    }

    static bool parse_bool(String bool_str) {
        return strbool_str.equals("1");
    }
}

// typedef struct SimulatedReturnVal {
//     uint8_t function_id;
//     String val_string;
// }