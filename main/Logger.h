#include <stdint.h>

class Logger {
  static const bool debugActivated = true;
  static const bool errorActivated = true;

  public:
    static void error(char* message) {
      if (errorActivated) {
        Serial.print("[Error] ");
        Serial.println(message);
      }
    }

    static void error(float value) {
      if (errorActivated) {
        Serial.print("[Error] ");
        Serial.println(value);
      }
    }

    static void error(String message) {
      if (errorActivated) {
        Serial.print("[Error] ");
        Serial.println(message);
      }
    }

    static void debug(char* message) {
      if (debugActivated) {
        Serial.print("[Debug] ");
        Serial.println(message);
      }
    }

    static void debug(float value) {
      if (debugActivated) {
        Serial.print("[Debug] ");
        Serial.println(value);
      }
    }

    static void debug(String message) {
      if (debugActivated) {
        Serial.print("[Debug] ");
        Serial.println(message);
      }
    }

    static void log(char* message) {
      Serial.print("[Log] ");
      Serial.println(message);
    }

    static void log(float value) {
      Serial.print("[Log] ");
      Serial.println(value);
    }

    static void log(String message) {
      Serial.print("[Log] ");
      Serial.println(message);
    }
};