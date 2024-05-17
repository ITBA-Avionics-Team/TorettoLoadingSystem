#ifndef TORETTOSIMULATIONLIB_H
#define TORETTOSIMULATIONLIB_H
#include "TorettoSimulationLib.h"
#endif // TORETTOSIMULATIONLIB_H

#include "SimulationScenarios.h"

class SimulationModule {

    int local_simulation_scenario_command_index = 0;
    int local_simulated_scenario_length = 13; // IMPORTANT: MODIFY WHEN MODIFYING LOCAL SIMULATED SCENARIO
    // TimedSimulationCommand local_simulated_scenario[] = NOMINAL_SCENARIO;

    public:
    String communication_module_return_vals[4] = {"","", "",""};
    String sensor_module_return_vals[7] = {"","","","","","", ""};
    String weather_module_return_vals[1] = {""};
    String storage_module_return_vals[1] = {""};

    SimulationModule() {
    
    }

    void loop() {
        // check_serial_for_simulation_commands();
        check_for_local_simulation_scenario_commands();
    }

    void check_serial_for_simulation_commands() {
        if (Serial.available()) {
            char buffer[50];
            int message_len = Serial.readBytesUntil('|', buffer, 40);
            SimulationCommand command = SimulationCommand::from_message(String(buffer).substring(0, message_len));
            save_simulation_command_value(command);
        }
    }

    void check_for_local_simulation_scenario_commands() {
        if (local_simulation_scenario_command_index >= local_simulated_scenario_length) return;

        if (millis() >= NOMINAL_STANDBY_SCENARIO[local_simulation_scenario_command_index].command_time_ms) {
            SimulationCommand command = NOMINAL_STANDBY_SCENARIO[local_simulation_scenario_command_index].command;
            save_simulation_command_value(command);
            // Serial.println(String("Saving command:") + command.value);
            local_simulation_scenario_command_index++;
        }
    }

    void save_simulation_command_value(SimulationCommand command) {
        switch (command.module) {
            case COMMUNICATION_MODULE:
                communication_module_return_vals[command.function_id] = command.value;
                break;
            case SENSOR_MODULE:
                sensor_module_return_vals[command.function_id] = command.value;
                break;
            case WEATHER_MODULE:
                weather_module_return_vals[command.function_id] = command.value;
                break;
            case STORAGE_MODULE:
                storage_module_return_vals[command.function_id] = command.value;
                break;
            default:
                // Serial.println(String("No module recognized:") + String(command.module));
                Logger::debug(String("No module recognized:") + String(command.module));

        }
    }

    static Command parse_command_str(String cmd_str) {
        return Command::from_message(cmd_str);
    }

    static OBECStatus parse_OBEC_status_str(String status_str) {
        return OBECStatus::from_message(status_str);
    }

    static int parse_int_str(String int_str) {
        return int_str.toInt();
    }

    static float parse_float_str(String float_str) {
        return float_str.toFloat();
    }

    static bool parse_bool_str(String bool_str) {
        return bool_str.equals("1");
    }

    static String bool_to_str(bool val) {
        return val ? String("1") : String("0");
    }
};
