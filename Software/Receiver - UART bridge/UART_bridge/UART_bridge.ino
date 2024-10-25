
/** Nordic UART Service Bridge:
 *
 *  Connects with a BLE device specified by name and bridges the Nordic UART Service to its serial port
 *
 *  Created: on April 2 2024
 *      Author: DieKatzchen
 *
*/

#include <NimBLEDevice.h>

#define GLOVENAME "lucidgloves-left"
#define LED 2
//#define _DEBUG //Uncomment to turn on debug messages

void scanEndedCB(NimBLEScanResults results);

static NimBLEAdvertisedDevice* advDevice;
static NimBLERemoteService* pSvc;
static NimBLERemoteCharacteristic* rxChr;
static NimBLERemoteCharacteristic* txChr;

static bool doConnect = false;
static bool connected = false;
static uint32_t scanTime = 0; /** 0 = scan forever */

static NimBLEScan* pScan;

class ClientCallbacks : public NimBLEClientCallbacks {
    void onDisconnect(NimBLEClient* pClient) {
        #ifdef _DEBUG
        Serial.print(pClient->getPeerAddress().toString().c_str());
        Serial.println(" Disconnected - Starting scan");
        #endif
        digitalWrite(LED, LOW);
        connected = false;

        NimBLEDevice::deleteClient(pClient);
        NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
    };
};

/** Define a class to handle the callbacks when advertisments are received */
class AdvertisedDeviceCallbacks: public NimBLEAdvertisedDeviceCallbacks {

    void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
        #ifdef _DEBUG
        Serial.print("Advertised Device found: ");
        Serial.println(advertisedDevice->toString().c_str());
        #endif
        std::string deviceName = advertisedDevice->getName();
        if(deviceName == GLOVENAME)
        {
            #ifdef _DEBUG
            Serial.println("Found Glove");
            #endif
            /** stop scan before connecting */
            NimBLEDevice::getScan()->stop();
            /** Save the device reference in a global for the client to use*/
            advDevice = advertisedDevice;
            /** Ready to connect now */
            doConnect = true;
        }
    };
};


/** Notification / Indication receiving handler callback */
void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
    Serial.print((char*)pData);
}

/** Callback to process the results of the last scan or restart it */
void scanEndedCB(NimBLEScanResults results){
    #ifdef _DEBUG
    Serial.println("Scan Ended");
    #endif
}

/** Create a single global instance of the callback class to be used by all clients */
static ClientCallbacks clientCB;

/** Handles the provisioning of clients and connects / interfaces with the server */
bool connectToServer() {
    NimBLEClient* pClient = nullptr;
    /** Check if we have a client we should reuse first **/
    if(NimBLEDevice::getClientListSize()) {
        // Special case when we already know this device, we send false as the
        //  second argument in connect() to prevent refreshing the service database.
        //  This saves considerable time and power.
        //
        pClient = NimBLEDevice::getClientByPeerAddress(advDevice->getAddress());
        if(pClient){
            if(!pClient->connect(advDevice, false)) {
                #ifdef _DEBUG
                Serial.println("Reconnect failed");
                #endif
                return false;
            }
            #ifdef _DEBUG
            Serial.println("Reconnected client");
            #endif
            //connected = true;
        }
        // We don't already have a client that knows this device,
        //  we will check for a client that is disconnected that we can use.
        //
        else {
            pClient = NimBLEDevice::getDisconnectedClient();
        }
    }

    /** No client to reuse? Create a new one. */
    if(!pClient) {
        if(NimBLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS) {
            #ifdef _DEBUG
            Serial.println("Max clients reached - no more connections available");
            #endif
            return false;
        }

        pClient = NimBLEDevice::createClient();
        #ifdef _DEBUG
        Serial.println("New client created");
        #endif
        pClient->setClientCallbacks(&clientCB, false);
        /** Set initial connection parameters: These settings are 15ms interval, 0 latency, 120ms timout.
         *  These settings are safe for 3 clients to connect reliably, can go faster if you have less
         *  connections. Timeout should be a multiple of the interval, minimum is 100ms.
         *  Min interval: 12 * 1.25ms = 15, Max interval: 12 * 1.25ms = 15, 0 latency, 51 * 10ms = 510ms timeout
         */
        pClient->setConnectionParams(12,12,0,51);
        /** Set how long we are willing to wait for the connection to complete (seconds), default is 30. */
        pClient->setConnectTimeout(5);


        if (!pClient->connect(advDevice)) {
            /** Created a client but failed to connect, don't need to keep it as it has no data */
            NimBLEDevice::deleteClient(pClient);
            #ifdef _DEBUG
            Serial.println("Failed to connect, deleted client");
            #endif
            return false;
        }
    }

    if(!pClient->isConnected()) {
        if (!pClient->connect(advDevice)) {
            #ifdef _DEBUG
            Serial.println("Failed to connect");
            #endif
            return false;
        }
    }

    #ifdef _DEBUG
    Serial.print("Connected to: ");
    Serial.println(pClient->getPeerAddress().toString().c_str());
    Serial.print("RSSI: ");
    Serial.println(pClient->getRssi());
    #endif

    /** Now we can read/write/subscribe the charateristics of the services we are interested in */
    pSvc = pClient->getService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    if(pSvc) {     /** make sure it's not null */
        txChr = pSvc->getCharacteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
        if(!txChr){
          #ifdef _DEBUG
          Serial.println("Nordic TX characteristic not found.");
          #endif
          return false;
        }
        /*if(txChr) {     // make sure it's not null
            if(txChr->canNotify()) {
                if(!txChr->subscribe(true, notifyCB)) {
                    // Disconnect if subscribe failed
                    pClient->disconnect();
                    return false;
                }
            }
        }*/
        rxChr = pSvc->getCharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
        if(!rxChr){
          #ifdef _DEBUG
          Serial.println("Nordic RX characteristic not found.");
          #endif
          return false;
        }
    } else {
        #ifdef _DEBUG
        Serial.println("Nordic UART service not found.");
        #endif
        return false;
    }

    //Serial.println("Done with this device!");
    doConnect = false;
    return true;
}

void setup (){
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    #ifdef _DEBUG
    Serial.println("Starting NimBLE Client");
    #endif
    /** Initialize NimBLE, no device name spcified as we are not advertising */
    NimBLEDevice::init("");
    
    /** Optional: set the transmit power, default is 3db */
#ifdef ESP_PLATFORM
    NimBLEDevice::setPower(ESP_PWR_LVL_P9); /** +9db */
#else
    NimBLEDevice::setPower(9); /** +9db */
#endif

    /** create new scan */
    //NimBLEScan* pScan = NimBLEDevice::getScan();
    pScan = NimBLEDevice::getScan();

    /** create a callback that gets called when advertisers are found */
    pScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());

    //pScan->setDuplicateFilter(false);
    //pScan->setMaxResults(0);
    /** Set scan interval (how often) and window (how long) in milliseconds */
    pScan->setInterval(45);
    pScan->setWindow(15);

    /** Active scan will gather scan response data from advertisers
     *  but will use more energy from both devices
     */
    pScan->setActiveScan(true);
    /** Start scanning for advertisers for the scan time specified (in seconds) 0 = forever
     *  Optional callback for when scanning stops.
     */
    pScan->start(scanTime, scanEndedCB);
}


void loop (){
  
    if(doConnect){ /** Found a device we want to connect to, do it now */
      if(connectToServer()) {
        #ifdef _DEBUG
        Serial.println("Success! we should now be getting notifications");
        #endif
        digitalWrite(LED, HIGH);
        connected = true;
      } else {
        #ifdef _DEBUG
        Serial.println("Failed to connect, starting scan");
        #endif
        digitalWrite(LED, LOW);
        connected = false;
        NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
      }
    } else if(connected){
      Serial.print(txChr->readValue().c_str());
      if (Serial.available()){      
        char message[100];
        byte size = Serial.readBytesUntil('\n', message, 100);
        message[size] = NULL;
        #ifdef _DEBUG
        Serial.print("sending: ");
        Serial.print(message);
        #endif
        rxChr->writeValue(message);
      }
    } else {
      vTaskDelay(1); //keep watchdog fed
    }
}
