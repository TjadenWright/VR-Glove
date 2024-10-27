# BLE 4.2 vs UWB
## BLE 4.2
* Pretty slow, at least compared to UWB. The data rate for serial is about 110 lines per second. BLE brings this data rate down to 34 lines per second or a 69.1% reduction.
<img src="https://github.com/TjadenWright/VR-Glove/blob/main/Images%20And%20Videos/ble4_2VsSerial.gif" alt="BLE 4.2" title="BLE 4.2" />

## UWB
* Very fast, basically as fast as serial. The data rate for serial os about 110 lines per second. UWB brings this down to 104 lines per second or only a 5.5% reduction. 
<img src="https://github.com/TjadenWright/VR-Glove/blob/main/Images%20And%20Videos/uwbVsSerial.gif" alt="UWB" title="UWB" />

## UWB Board
* The board I used was the [ESP32 UWB Pro](https://www.makerfabs.com/esp32-uwb-high-power-120m.html). This also has the added benifit of having a range of ~200m, which is pretty crazy.
* If you have this board all you need to do is install [this libaray](https://github.com/TjadenWright/VR-Glove/tree/main/Software/UWB%20VR%20Glove%20Communication/README.md) and set your device to an DOIT ESP32 DEV Board. 