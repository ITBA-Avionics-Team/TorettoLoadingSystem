
#include "Arduino.h"
//#include "heltec.h"

class OLEDModule {

  public:
  static void init() {
    //Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
    //Heltec.display->flipScreenVertically();
    //Heltec.display->setFont(ArialMT_Plain_10);
  }

  static void printText(String text) {
    //Heltec.display->clear();
    //Heltec.display->setFont(ArialMT_Plain_10);
    //Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    //Heltec.display->drawStringMaxWidth(0, 0, 128, text);
    //Heltec.display->display();
  }

};