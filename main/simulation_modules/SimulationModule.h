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
            int module_id_len = Serial.readBytesUntil(',', buffer, 10);
            if (module_id_len != 2){
                Logger::error("[SimModule] Message received with module_id of different length than 2 chars.");
            }
            String module_id = String(buffer).substring(0, module_id_len);

            int function_id_len = Serial.readBytesUntil(',', buffer, 10);
            int function_id = String(buffer).substring(0, function_id_len).toInt();
            String value = Serial.readString();
            // TODO: Check if the value has a newline at the end and send a warning, or strip it.
            if (module_id.equals("CM")) {
                // Logger::debug("[SM]Received CM data for functionId " + String(function_id) + String(" - ") + value);
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
        Logger::debug("Parsing obec status..." + status_str);
        return OBECStatus::from_message(status_str);
    }

    static bool parse_bool(String bool_str) {
        return bool_str.equals("1");
    }

    static String bool_to_str(bool val) {
        return val ? String("1") : String("0");
    }
};
