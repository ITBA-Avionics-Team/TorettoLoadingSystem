
#ifndef LOGGER_H
#define LOGGER_H
#include "Logger.h"
#endif // LOGGER_H

class SensorModule {
  public:

  void init() {
    Logger::log("Initializing sensor Module...");

  
    Logger::log("Sensor Module initialized.");
  }


bool umbrilical_connected() {
  return true; //TODO: Implement
}


};
