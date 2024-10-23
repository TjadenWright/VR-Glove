### Nordic UART Service Bridge ###
This firmaware will connect automatically to a BLE device with the specified name, attempt to connect to a Nordic UART Service, and then bridge it to its own UART port (which on most development kits is in turn connected to a UART to USB bridge)
The main branch is designed with the needs of the [Lucidgloves Fae Mod](https://github.com/codingcatgirl/lucidgloves-fae-mod) in mind, specifically that we're not subscribing to the TX characteristic and instead simply check it when we're ready to. This of course results in missed messages, if your use case requires receiving all incoming messages, try the general-use branch.

To compile, will require the NimBLE-Arduino library, it is available in the library manager. Remember to define the name of your device in GLOVENAME. Uncomment the _DEBUG define in order to see debug messages through serial.
