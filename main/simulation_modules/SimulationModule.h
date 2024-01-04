
class SimulationModule {

    public:
    String communication_module_return_vals[4] = {"","", "", ""};
    String sensor_module_return_vals[2] = {"",""};
    String weather_module_return_vals[1] = {""};
    String storage_module_return_vals[1] = {""};

    void loop() {
        if (Serial.available()) {
            char buffer[50];
            String module_id = Serial.readBytesUntil(',', &buffer, 10);
            int function_id = Serial.parseInt();
            String value = Seria.readString();
            switch(module_id) {
                case "CM":
                    communication_module_return_vals[function_id] = value;
                    break;
                case "SE":
                    sensor_module_return_vals[function_id] = value;
                    break;
                case "WE":
                    weather_module_return_vals[function_id] = value;
                    break;
                case "ST":
                    storage_module_return_vals[function_id] = value;
                    break;
            }
        }
    }

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