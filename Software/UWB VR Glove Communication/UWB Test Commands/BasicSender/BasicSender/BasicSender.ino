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
 * @file BasicSender.ino
 * Use this to test simple sender/receiver functionality with two
 * DW1000. Complements the "BasicReceiver" example sketch. 
 * 
 * @todo
 *  - move strings to flash (less RAM consumption)
 *  
 */

//uwb
#define UWBSERIAL_DEVICE_ADDRESS 5
#define UWBSERIAL_NETWORK_ID     10
#define UWB_CHANNEL              5

#include <SPI.h>
#include <DW1000.h>

// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4; // spi select pin

// DEBUG packet sent status and count
volatile boolean sentAck = false;
char* msg_input = "A2465B2474C2450D2447E2463F2047G1618P854ILM(AB)2481(BB)1881(CB)1818(DB)1857(EB)1861";
DW1000Time sentTime;

void setup() {

  // DEBUG monitoring
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("### DW1000-arduino-sender-test ###"));
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
  char msg[128];
  DW1000.getPrintableDeviceIdentifier(msg);
  Serial.print("Device ID: "); Serial.println(msg);
  DW1000.getPrintableExtendedUniqueIdentifier(msg);
  Serial.print("Unique ID: "); Serial.println(msg);
  DW1000.getPrintableNetworkIdAndShortAddress(msg);
  Serial.print("Network ID & Device Address: "); Serial.println(msg);
  DW1000.getPrintableDeviceMode(msg);
  Serial.print("Device mode: "); Serial.println(msg);
  // attach callback for (successfully) sent messages
  DW1000.attachSentHandler(handleSent);
  // start a transmission
  output(msg_input);
}

void handleSent() {
  // status change on sent success
  sentAck = true;
}

void output(char* data) {
  // transmit some data
  DW1000.newTransmit();
  DW1000.setDefaults();
  DW1000.setData(data);
  // delay sending the message for the given amount
  DW1000.startTransmit();
}

void loop() {
  if (!sentAck) {
    return;
  }
  // continue on success confirmation
  // (we are here after the given amount of send delay time has passed)
  sentAck = false;
  // update and print some information about the sent message
  DW1000Time newSentTime;
  DW1000.getTransmitTimestamp(newSentTime);
  // note: delta is just for simple demo as not correct on system time counter wrap-around
  Serial.print("DW1000 delta send time [ms] ... "); Serial.println((newSentTime.getAsMicroSeconds() - sentTime.getAsMicroSeconds()) * 1.0e-3);
  sentTime = newSentTime;

  // again, transmit some data
  output(msg_input);
}
