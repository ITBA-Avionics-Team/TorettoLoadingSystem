#ifndef OLED_H
#define OLED_H
#include "OLEDModule.h"
#endif // OLED_H
#include <stdint.h>

class Logger {
  static const bool debugActivated = true;
  static const bool errorActivated = true;

  public:
    static void error(float value) {
      if (errorActivated) {
        // Serial.print("[Error] ");
        // Serial.println(value);
        OLEDModule::printText("[Error] " + String(value));
      }
    }

    static void error(String message) {
      if (errorActivated) {
        // Serial.print("[Error] ");
        // Serial.println(message);
        OLEDModule::printText("[Error] " + message);
      }
    }

    static void debug(float value) {
      if (debugActivated) {
        // Serial.print("[Debug] ");
        // Serial.println(value);
        OLEDModule::printText("[Debug] " + String(value));
      }
    }

    static void debug(String message) {
      if (debugActivated) {
        // Serial.print("[Debug] ");
        // Serial.println(message);
        OLEDModule::printText("[Debug] " + message);
      }
    }

    static void log(float value) {
      // Serial.print("[Log] ");
      // Serial.println(value);
      OLEDModule::printText("[Log] " + String(value));
    }

    static void log(String message) {
      // Serial.print("[Log] ");
      // Serial.println(message);
      OLEDModule::printText("[Log] " + message);
    }


};