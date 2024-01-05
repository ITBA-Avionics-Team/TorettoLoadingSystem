class SimulationModule {

    public:
    String communication_module_return_vals[4] = {"","", "", ""};
    String sensor_module_return_vals[2] = {"",""};
    String weather_module_return_vals[1] = {""};
    String storage_module_return_vals[1] = {""};

    SimulationModule() {
    
    }

    void loop() {
        if (Serial.available()) {
            char buffer[50];
            Serial.readBytesUntil(',', buffer, 10);
            String module_id = String(buffer).substring(0, 2);
            int function_id = Serial.parseInt();
            Serial.read(); // Read the comma to discard it
            String value = Serial.readString();
            // TODO: Check if the value has a newline at the end and send a warning, or strip it.
            if (module_id.equals("CM")) {
                communication_module_return_vals[function_id] = value;
            }
            else if(module_id.equals("SE")) {
                sensor_module_return_vals[function_id] = value;
            }
            else if (module_id.equals("WE")) {
                weather_module_return_vals[function_id] = value;
            }
            else if (module_id.equals("ST")) {
                storage_module_return_vals[function_id] = value;
            } else {
                Logger::debug(String("No module recognized:") + String(module_id));
            }
        }
    }

    static Command parse_command(String cmd_str) {
        if (cmd_str.substring(0,2).equals("SS")){
            String state_str = cmd_str.substring(2,6);
            if (state_str.equals("LDNG")){
                return Command(SwitchStateCommand, LOADING);
            }
        }
        return Command();
    }

    static OBECStatus parse_OBEC_status(String status_str) {
        return OBECStatus();
    }

    static bool parse_bool(String bool_str) {
        return bool_str.equals("1");
    }

    static String bool_to_str(bool val) {
        return val ? String("1") : String("0");
    }
};
