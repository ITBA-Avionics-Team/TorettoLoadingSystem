// void ContainerCommunicationModule::init(Stream& groundXBeeSerial, Stream& payloadsXBeeSerial) {
//   // Start the serial port
//   groundXBee.setSerial(groundXBeeSerial);
// //  payloadsXBee.setSerial(payloadsXBeeSerial);
// //  Serial.begin(19200);
// //  EEPROM.get(PACKET_COUNT_EEPROM_ADDR, packetCount);
// }

// void ContainerCommunicationModule::parseReceivedPacket(uint8_t* packetData, uint8_t packetLength) {
//   Serial.write("Parse:");
//   Serial.write(packetData, packetLength);
//   Serial.write("\n");
//   if (packetData[0] == 'C') {
//     parseCommandPacket(packetData, packetLength);
//   } else {
//     parseTelemetryPacket(packetData, packetLength);
//   }
// }

// void ContainerCommunicationModule::parseCommandPacket(uint8_t* packetData, uint8_t packetLength) {
//   /*
//     Examples:
//     CMD,2764,CX,ON activates Container telemetry
//     CMD,2764,ST,13:35:59 sets the mission time to the value given (we already parse these commmands using setRtcTimeFromPacket)
//     CMD,2764,SIM,ENABLE and CMD,1000,SIM,ACTIVATE commands are required to begin simulation mode.
//     CMD,2764,SP1X,ON will trigger the Container to relay a command to theScience Payload 1 to begin telemetry transmissions.
//     CMD,1000,SIMP,101325 provides a simulated pressure reading to the Container (101325 Pascals = approximately sea level). Note: this command is to be used only in simulation mode.
//   */
//   if (packetData[9] == 'C') { //CX command
//     if (packetData[12] == 'O' && packetData[13] == 'N')
//       Serial.write("CXON\n");
//     else
//       Serial.write("CXOFF\n");
//     setContainerTelemetryActivated(packetData[12] == 'O' && packetData[13] == 'N');
//   }
//   else if (packetData[9] == 'S' && packetData[10] == 'I') { //SIM or SIMP command
//     if (packetData[12] == ',') { //SIM command
//       if (packetData[13] =='D') {
// //        Serial.write("SIMD\n");
//         setContainerSimulationMode(SIMULATION_DISABLED);
//       }
//       else if (packetData[13] =='E') {
// //        Serial.write("SIME\n");
//         setContainerSimulationMode(SIMULATION_ENABLED);
//       }
//       else {
// //        Serial.write("SIMA\n");
//         setContainerSimulationMode(SIMULATION_ACTIVATED);
//       }
//     } else { //SIMP command
//       uint8_t pressureValueBuffer[8];
//       uint8_t i = 0;
//       while (i < 8)pressureValueBuffer[i++]=0;
//       for (i = 14; i < packetLength; i++) {
//         pressureValueBuffer[i-14] = packetData[i];
//       }
//       pressureValueBuffer[i] = 0;
//       setLatestSimulationPressureValue(atof(pressureValueBuffer));
//     }
//   }
//   else if (packetData[9] == 'S' && packetData[10] == 'P') { //SP1X or SP2X command
//     if (packetData[14] == 'O' && packetData[15] == 'N'){
//       if (packetData[11] == '1') {
// //        Serial.write("SP1XON\n");
//         payload1CommandQueue.add('1');
//       } else {
// //        Serial.write("SP2XON\n");
//         payload2CommandQueue.add('1');
//       }
//     } else {
//       if (packetData[11] == '1') {
// //        Serial.write("SP1XOFF\n");
//         payload1CommandQueue.add('0');
//       } else {
// //        Serial.write("SP2XOFF\n");
//         payload2CommandQueue.add('0');
//       }
//     }
//   }
//   else if (packetData[9] == 'S' && packetData[10] == 'T') { //ST command
//     setRtcTimeFromCommandPacket(packetData, packetLength);
//   } 
//   uint8_t j = 0;
//   for (uint8_t i=0; i < packetLength;i++){ 
//     if (packetData[i] != ',') lastCommandEcho[j++] = packetData[i];
//   }
//   lastCommandEcho[j] = 0;
// }

// // 2764,20:02:03,1232,S1, 23.4,19.2,200
// void ContainerCommunicationModule::parseTelemetryPacket(uint8_t* packetData, uint8_t packetLength) {
//   if (packetData[20] == '1') {
//     memcpy(latestPayload1Packet, packetData, packetLength);
//     memcpy(latestPayload1Packet + 5, generateMissionTimeString(), 8);
//     telemetryPacketQueue.add(latestPayload1Packet, packetLength);
//   }
//   else if (packetData[20] == '2') {
//     memcpy(latestPayload2Packet, packetData, packetLength);
//     memcpy(latestPayload2Packet + 5, generateMissionTimeString(), 8);
//     telemetryPacketQueue.add(latestPayload2Packet, packetLength);
//   }
// }

// void ContainerCommunicationModule::sendNextPayload1Command() {
//   payloadsRequestObj = ZBTxRequest(payload1Address, payload1CommandQueue.head->data, sizeof(uint8_t));
//   payloadsXBee.send(payloadsRequestObj);
// }

// void ContainerCommunicationModule::sendNextPayload2Command() {
//   payloadsRequestObj = ZBTxRequest(payload2Address, payload2CommandQueue.head->data, sizeof(uint8_t));
//   payloadsXBee.send(payloadsRequestObj);
// }

// void ContainerCommunicationModule::sendNextTelemetryPacket(){
//   Serial.write(telemetryPacketQueue.head->data, telemetryPacketQueue.head->dataLength);
//   Serial.write('\n');
//   groundRequestObj = ZBTxRequest(groundAddress, telemetryPacketQueue.head->data, telemetryPacketQueue.head->dataLength);
//   groundXBee.send(groundRequestObj);
//   packetCount++;
//   EEPROM.put(PACKET_COUNT_EEPROM_ADDR, packetCount);
// }

// void ContainerCommunicationModule::loop() {
//   manageGroundCommunication();
// }

// void ContainerCommunicationModule::manageGroundCommunication() {
//   int groundReceiveStatus = receivePackets(groundXBee, groundResponseObj, groundRequestStatusObj);
//   if (groundReceiveStatus == ZB_RX_RESPONSE) { // We received a message so we parse it and act on it.
//     uint8_t* packetData = groundResponseObj.getData();
//     uint8_t dataLength = groundResponseObj.getDataLength();
//     parseReceivedPacket(packetData, dataLength);
//      if (groundResponseObj.getOption() == ZB_PACKET_ACKNOWLEDGED) {
//        //console.log('ST command packet received and acknowledged');
//      } else {
//        //console.log('ST command packet received but sender didnt get an ack');
//      }
//   } else if (groundReceiveStatus == ZB_TX_STATUS_RESPONSE) { // We received a status update on a previously sent packet
//     if (groundRequestStatusObj.getDeliveryStatus() == SUCCESS) { // We got an ACK Wohoo!
//       Serial.println("Ack biatch");
//       groundCommunicationState = STATE_IDLE;
//     } else { //We got a status response but it wasn't an ACK, so we resend the packet
//       sendNextTelemetryPacket();
//     }
//   } else {
// //    Serial.write("Container received packet of type: ");
// //    Serial.write(groundReceiveStatus);
// //      Serial.write("No response");
//   }
//   if (groundCommunicationState == STATE_IDLE && !telemetryPacketQueue.isEmpty()) {
//     Serial.write("Send:");
//     sendNextTelemetryPacket();
//     telemetryPacketQueue.removeHead();
//   }
// }


// int CommunicationModule::receivePackets(XBee& xbee, ZBRxResponse& responseObj, ZBTxStatusResponse& requestStatusObj) {
//   xbee.readPacket();
//   if (xbee.getResponse().isAvailable()) {
// //    Serial.write("Got XBEE Response:\n");
//     if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {    
//       xbee.getResponse().getZBRxResponse(responseObj);
// //      Serial.write("ZB_RX_RESPONSE\n");
//       if (responseObj.getOption() == ZB_PACKET_ACKNOWLEDGED) {
//         return ZB_RX_RESPONSE;
//       } else {  
//         // we got it (obviously) but sender didn't get an ACK
//       }
//     } else if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
//       xbee.getResponse().getZBTxStatusResponse(requestStatusObj);
//       Serial.write("ZB_TX_STATUS_RESPONSE");
//       return ZB_TX_STATUS_RESPONSE;
//     } else if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
//       Serial.write("AT_COMMAND_RESPONSE");
//       xbee.getResponse().getAtCommandResponse(atCommandResponse);
//       return AT_COMMAND_RESPONSE;
//     }
//   } else if (xbee.getResponse().isError()) {
// //    Serial.write("Error reading packet.  Error code: ");  
// //    Serial.write(xbee.getResponse().getErrorCode());
//   } else {
// //    Serial.write("No response from XBEE :(\n");
//   }
    
 
//   return false;
// }