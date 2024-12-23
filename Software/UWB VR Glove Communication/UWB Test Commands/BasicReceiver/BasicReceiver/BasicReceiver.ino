/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net>
 * Decawave DW1000 library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file BasicReceiver.ino
 * Use this to test simple sender/receiver functionality with two
 * DW1000. Complements the "BasicSender" example sketch.
 * 
 * @todo
 *  - move strings to flash (less RAM consumption)
 *  
 * Edited: Tjaden Wright - Added support for LucidGlove firmware (Left Glove).
 */

#define UWBSERIAL_DEVICE_ADDRESS 5
#define UWBSERIAL_NETWORK_ID     10
#define UWB_CHANNEL              4

#include <SPI.h>
#include <DW1000.h>

// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4; // spi select pin

// DEBUG packet sent status and count
volatile boolean received = false;
volatile boolean error = false;
volatile int16_t numReceived = 0; // todo check int type
String message;

void setup() {
  // DEBUG monitoring
  Serial.begin(115200);
  Serial.println(F("### DW1000-arduino-receiver-test ###"));
  // initialize the driver
  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);
  Serial.println(F("DW1000 initialized ..."));
  // general configuration
  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setDeviceAddress(UWBSERIAL_DEVICE_ADDRESS);
  DW1000.setNetworkId(UWBSERIAL_NETWORK_ID);
  DW1000.setChannel(UWB_CHANNEL);
  DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
  DW1000.commitConfiguration();
  Serial.println(F("Committed configuration ..."));
  // DEBUG chip info and registers pretty printed
  char msg[2048];
  DW1000.getPrintableDeviceIdentifier(msg);
  Serial.print("Device ID: "); Serial.println(msg);
  DW1000.getPrintableExtendedUniqueIdentifier(msg);
  Serial.print("Unique ID: "); Serial.println(msg);
  DW1000.getPrintableNetworkIdAndShortAddress(msg);
  Serial.print("Network ID & Device Address: "); Serial.println(msg);
  DW1000.getPrintableDeviceMode(msg);
  Serial.print("Device mode: "); Serial.println(msg);
  // attach callback for (successfully) received messages
  DW1000.attachReceivedHandler(handleReceived);
  DW1000.attachReceiveFailedHandler(handleError);
  DW1000.attachErrorHandler(handleError);
  // start reception
  receiver();
}

void handleReceived() {
  // status change on reception success
  received = true;
}

void handleError() {
  error = true;
}

void receiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  // so we don't need to restart the receiver manually
  DW1000.receivePermanently(true);
  DW1000.startReceive();
}

String extractSubstring(String input, char startChar, char endChar) {
    int startIndex = input.indexOf(startChar);
    int endIndex = input.indexOf(endChar, startIndex + 1);

    if (startIndex == -1 || endIndex == -1) {
        return ""; // Return empty string if start or end character is not found
    }

    return input.substring(startIndex, endIndex); // Include the end character
}

bool isLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

String extractNumberAfterLetter(String input, char letter) {
    int startIndex = input.indexOf(letter);
    if (startIndex == -1) {
        return ""; // Letter not found in the string
    }

    String result = "";
    for (int i = startIndex + 1; i < input.length(); i++) {
        if (isDigit(input[i])) {
            result += input[i];
        } else if (isLetter(input[i])) {
            break; // Stop when the next letter is encountered
        }
    }

    return result;
}

void loop() {
  // enter on confirmation of ISR status change (successfully received)
  if (received) {
    numReceived++;
    // get data as string
    DW1000.getData(message);
    // Serial.print("Received message ... #"); Serial.println(numReceived);
    String cleanMessage = extractSubstring(message, 'A', 'Q');
    cleanMessage = cleanMessage + "(AB)" + String(extractNumberAfterLetter(message, 'Q'));
    cleanMessage = cleanMessage + "(BB)" + String(extractNumberAfterLetter(message, 'R'));
    cleanMessage = cleanMessage + "(CB)" + String(extractNumberAfterLetter(message, 'S'));
    cleanMessage = cleanMessage + "(DB)" + String(extractNumberAfterLetter(message, 'T'));
    cleanMessage = cleanMessage + "(EB)" + String(extractNumberAfterLetter(message, 'U'));
    Serial.println(cleanMessage);
    // Serial.print("FP power is [dBm] ... "); Serial.println(DW1000.getFirstPathPower());
    // Serial.print("RX power is [dBm] ... "); Serial.println(DW1000.getReceivePower());
    // Serial.print("Signal quality is ... "); Serial.println(DW1000.getReceiveQuality());
    received = false;
  }
  if (error) {
    Serial.println("Error receiving a message");
    error = false;
    DW1000.getData(message);
    Serial.print("Error data is ... "); Serial.println(message);
  }
}
