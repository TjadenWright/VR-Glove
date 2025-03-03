//only compiles if BLESerial is set because it won't compile for a non-compatible board
#if COMMUNICATION == COMM_UWBSERIAL
#include <SPI.h>
#include <DW1000.h>
volatile boolean sentAck = false;
class UWBSerialCommunication : public ICommunication {
  private:
    bool m_isOpen;
    
    const uint8_t PIN_RST = 27; // reset pin
    const uint8_t PIN_IRQ = 34; // irq pin
    const uint8_t PIN_SS = 4; // spi select pin

    DW1000Time sentTime;

    static void handleSent(){
      // status change on sent success
      sentAck = true;
    }
  public:

    UWBSerialCommunication() {
      m_isOpen = false;
    }

    bool isOpen(){
      return m_isOpen;
    }

    void start(){
      // initialize the driver
      DW1000.begin(PIN_IRQ, PIN_RST);
      DW1000.select(PIN_SS);
      // general configuration
      DW1000.newConfiguration();
      DW1000.setDefaults();
      DW1000.setDeviceAddress(UWBSERIAL_DEVICE_ADDRESS);
      DW1000.setNetworkId(UWBSERIAL_NETWORK_ID);
      DW1000.setChannel(UWB_CHANNEL);
      DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
      DW1000.commitConfiguration();

      DW1000.attachSentHandler(handleSent);
      // start a transmission
        // transmit some data
        DW1000.newTransmit();
        DW1000.setDefaults();
        DW1000.setData("");
        // delay sending the message for the given amount
        DW1000.startTransmit();

      #if BT_ECHO
      Serial.begin(SERIAL_BAUD_RATE);
      Serial.println("The device started, now you can pair it with bluetooth!");
      #endif

      m_isOpen = true;
    }

    void output(char* data){
      if (!sentAck) {
        return;
      }

      sentAck = false;

      DW1000Time newSentTime;
      DW1000.getTransmitTimestamp(newSentTime);
      // note: delta is just for simple demo as not correct on system time counter wrap-around
      sentTime = newSentTime;

      // transmit some data
      DW1000.newTransmit();
      DW1000.setDefaults();
      DW1000.setData(data);
      // delay sending the message for the given amount
      DW1000.startTransmit();

      #if BT_ECHO
      Serial.print(data);
      Serial.flush();
      #endif
    }

    bool readData(char* input){
      // byte size = Serial.readBytesUntil('\n', input, 100);
      // input[size] = NULL;
      return input != NULL && strlen(input) > 0;
    }
};
#endif