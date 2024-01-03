
class RS485Module {
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

    OBECStatus check_for_status_message() {
      return OBECStatus();
    }

    void send_valve_command(Command command) {

    }
};
