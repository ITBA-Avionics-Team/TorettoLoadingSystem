#include <stdlib.h>
#include <stdint.h>

#include <Preferences.h>
#ifndef TORETTOLIB_H
#define TORETTOLIB_H
#include "../TorettoLib.h"
#endif // TORETTOLIB_H

#define CURRENT_STATE_KEY "curr_state"
#define PACKET_COUNT_KEY "packet_count"

class StorageModule {
  Preferences preferences;
  public:
    uint16_t packetCount = 0;

    void init(){
      preferences.begin("lanzamiento00", false);
      // Uncomment the line below to erase flash memory on init
      preferences.clear();
      
      Logger::log("Storage Module initialized.");
      packetCount = loadTelemetryPacketCount();
    }

    State load_current_state(State defaultState) {
      // return STANDBY; //TODO: Implement
      State savedState = (State)preferences.getUChar(CURRENT_STATE_KEY, 255); // 255 is the default value returned if no saved state is found
      if (savedState == 255) {
        Logger::debug("No previous state found in memory, defaulting to " + String(defaultState));
        return defaultState;
      } else {
        Logger::debug("Found previous state in memory: " + String(savedState));
        return savedState;
      }
    }

    void saveCurrentState(State currentState) {
      preferences.putUChar(CURRENT_STATE_KEY, currentState);
      Logger::debug("Saved current state:" + String(currentState));
    }

    void addTelemetryPacketToFlightLog(char* telemetryPacket) {
      
      char blobIdStr[4];
      sprintf(blobIdStr, "%03d", packetCount / 20);

      String currentBlobContent = preferences.getString(blobIdStr, "");
      preferences.putString(blobIdStr, currentBlobContent + String(telemetryPacket));
      saveTelemetryPacketCount(packetCount);
      packetCount++;
      Logger::debug("Saving telemetry packet to storage: " + String(telemetryPacket) + "- Blob id: " + String(blobIdStr));
    }

    uint16_t loadTelemetryPacketCount() {
      uint16_t savedPacketCount = preferences.getUShort(PACKET_COUNT_KEY, 0);
      Logger::debug("Packet count in memory: " + String(savedPacketCount));
      return savedPacketCount;
    }

    void saveTelemetryPacketCount(uint16_t count) {
      preferences.putUShort(PACKET_COUNT_KEY, count);
      Logger::debug("Saved current telemetry package count:" + String(count));
    }


};