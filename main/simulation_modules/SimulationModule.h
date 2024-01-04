
class SimulationModule {

    public:
    String communication_module_return_vals[2] = {"",""};
    String sensor_module_return_vals[2] = {"",""};
    String weather_module_return_vals[1] = {""};
    String storage_module_return_vals[1] = {""};

    // We should probable include parsing functions as a part of the SimulationModule (as static functions?)
}

// typedef struct SimulatedReturnVal {
//     uint8_t function_id;
//     String val_string;
// }