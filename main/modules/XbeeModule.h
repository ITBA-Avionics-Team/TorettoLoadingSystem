#include "Queue.h"
#include <stdlib.h>
#include <stdint.h>
#include <XBee.h>

#define STATE_IDLE 0
#define STATE_WAITING_FOR_GROUND_RESPONSE 1
#define STATE_WAITING_FOR_PAYLOAD_1_RESPONSE 2
#define STATE_WAITING_FOR_PAYLOAD_2_RESPONSE 3

#define SIMULATION_DISABLED 0
#define SIMULATION_ENABLED 1
#define SIMULATION_ACTIVATED 2

#define PAYLOAD_MAX_PACKAGE_LENGTH 42

#define PACKET_COUNT_EEPROM_ADDR 4

class XBeeModule {
  public:
    // uint16_t packetCount = 0;

    // XBee mccXBee = XBee();;
    // ZBRxResponse mccResponseObj = ZBRxResponse();
    // ZBTxRequest mccRequestObj;
    // ZBTxStatusResponse mccRequestStatusObj = ZBTxStatusResponse();

    // CommandQueue obecCommandQueue = CommandQueue();
    // uint8_t latestPayload1Packet[PAYLOAD_MAX_PACKAGE_LENGTH];

    // void XbeeModule::init(Stream& groundXBeeSerial);

    // void setRtcTimeFromPacket(uint8_t* packetData, uint8_t packetLength);
    // void parseReceivedPacket(uint8_t* packetData, uint8_t packetLength);
    // void parseCommandPacket(uint8_t* packetData, uint8_t packetLength);
    // void parseTelemetryPacket(uint8_t* packetData, uint8_t packetLength);

    // void sendNextPayload1Command();
    // void sendNextPayload2Command();
    // void sendNextTelemetryPacket();

    // void get_xbee_data() {
      
    // }
  
    // void managePayloadsCommunication();

    // void loop();

    Command check_for_commands(){
      return Command(ValveCommand, Open, ENGINE_VALVE);
    }

    void check_for_status_message() {

    }

    void send_system_status(SystemStatus system_status){

    }

    void send_preflight_check(PreflightCheckData data) {

    }

    void send_tank_depress_vent_tamp_low() {

    }

    void send_external_vent_as_default_prompt(){

    }

    void send_ignition_confirmation() {

    }
};
