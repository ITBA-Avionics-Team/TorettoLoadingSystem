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
            String module_id = String(buffer);
            int function_id = Serial.parseInt();
            String value = Serial.readString();
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
        return Command();
    }

    static OBECStatus parse_OBEC_status(String status_str) {
        return OBECStatus();
    }

    static bool parse_bool(String bool_str) {
        return bool_str.equals("1");
    }
};
