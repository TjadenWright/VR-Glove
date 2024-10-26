# Wiring Diagram
## Glove + Joystick
* I mainly used [this](https://github.com/LucidVR/lucidgloves/wiki/Prototype-5-Wiring-Guide) wiring diagram for the glove and joystick.
<img src="https://github.com/TjadenWright/VR-Glove/blob/main/Images%20And%20Videos/GloveWiringLucas.png" alt="Glove Wiring" title="Glove Wiring" />

## Battery Pack
* For the battery circuit I used [this](https://docs.slimevr.dev/diy/tracker-schematics.html).
* For my specific ESP32 I needed to use a [boost converter](https://www.amazon.com/gp/product/B07L76KLRY/ref=ox_sc_act_title_3?smid=A1IU4FC9F8Y7P1&psc=1) to boost the batteries 3.7v to 5v.
<img src="https://github.com/TjadenWright/VR-Glove/blob/main/Images%20And%20Videos/ChargeCircuit.png" alt="Charge Circuit" title="Charge Circuit" />

# Parts List
## Base Glove
* 2x ESP32. I used [this](https://www.makerfabs.com/esp32-uwb-high-power-120m.html), I don't recoment that one. I just had that from another project. I recommend [this one](https://www.amazon.com/ESP-WROOM-32-Development-Microcontroller-Integrated-Compatible/dp/B0BK13HWBJ?th=1).
* 10x [Hall Effect Sensors](https://www.amazon.com/gp/aw/d/B09MSDC3GR?psc=1&ref=ppx_pop_mob_b_asin_title).
* 10x [Magnets](https://www.amazon.com/gp/aw/d/B08NZTN426?psc=1&ref=ppx_pop_mob_b_asin_title).
* 1x [Multiplexer](https://www.amazon.com/gp/aw/d/B07K7JF3HX?psc=1&ref=ppx_pop_mob_b_asin_title).
* [Wires](https://www.amazon.com/Elegoo-EL-CP-004-Multicolored-Breadboard-arduino/dp/B01EV70C78/ref=sr_1_3?dib=eyJ2IjoiMSJ9.0RNBCjSRr0tpyeak5txbX2K5iinltlK4WF-CRFcMlKtMvW06Nb1z_CeoJXYjxmhOht4G1rvVBd5kUgXwMBgdRa0bdxVvhpRpFPsEO3fUGAXOQ4Pf7VOEjBt1fcKo_t0VB8riuPT_JoUKshtJbUuy5HoAw86FT-GE27nniNswwNQ-nc3qZD5l-IadwMNiWcVmONziBtwqbRNpyNAxL9fU7EmgSi3McVjEtd0t8FrzvRU.KDiwtTP1AaA9PLRFwe6PBQrrRJLGcmzRsoZc1nj7w_M&dib_tag=se&keywords=Arduino+Wire&qid=1729963019&sr=8-3).
* [M2 Screws](https://www.amazon.com/your-orders/pop?ref=ppx_yo2dv_mob_b_pop__np_0_pp&orderId=114-8647069-4298634&lineItemId=jgsotpqpmjnsoups&shipmentId=GkJxVSkZ8&packageId=1&asin=B0B93G1H9L).
* (Optional): [Cable Sleeves](https://www.amazon.com/HERCULES-Paracord-Parachute-Black-Survival/dp/B0BD2J2DQV/ref=sr_1_9?crid=1IZN8U3271YSA&dib=eyJ2IjoiMSJ9.DJ1M9JBshmhyWnkKDqiGbRBnV9cit0cVdYRV3ALZbHBNp-ZS4NB3E6pHPcTBMIIKM5958UwUQGA37yJd9m64zb3TV7_WNADZkvij0UrRQzSgP5p9jYEyhH1um_txJjtL82mQe13NP_m9Gj5nz2c6tOA-Li_VxNpWgTyBEeIxRxS2Y3ztmbvLO5qX5vW0A5vsX3cx37NFRj25xlic2WiKtlzFBme7cabkXzOzt7c1iVrVDLtGImUmeY5w9E6NQfhbxXtoAtaLaUkUXlZm8A36Qro8NtexHkqWq2HUxGe7yBM.KF24eSUxjL8StfepP6aDEpyY9BYlwgExjSKpx6m_XOg&dib_tag=se&keywords=paracord&qid=1729963341&sprefix=paracor%2Caps%2C168&sr=8-9).


## Joystick Upgrade
* 1x [Hall Joystick](https://www.aliexpress.us/item/3256806013522128.html?spm=a2g0o.order_list.order_list_main.5.189518022KC82G&gatewayAdapt=glo2usa).
* 2x [Buttons](https://www.amazon.com/DAOKI-100Pcs-6x6x5mm-Momentary-Tactile/dp/B07X8T9D2Q/ref=sr_1_16?crid=223KZWP4V3LLQ&dib=eyJ2IjoiMSJ9.mM1hL2Wa0yy1Rlv0aRKX5FbPO8WPTWKv06JMHIpE3OJNnTvdOLzV5Gkg9cVOui3rp1QTcybEymAwUTdC03fT2D0vQNUJTSgA41VYBMr69o96SHkhyVTbHqzW7LN84rfPi0NE-qUY4l526XMdWISBBU_Sb1AmM8Vt0QQUHW4HBGKKjSEvv0evFrQSGuiXPd7pMAAvrMqv0x2jsmw6efmao3vVEzZCJixA2i8eaIoZYhQ.ZfXumC9jycFyjfJcdhznyuiRwmexbJNdo1fIgkt4X9w&dib_tag=se&keywords=buttons+electrical&qid=1729963864&sprefix=buttons+electrical%2Caps%2C152&sr=8-16).
* 1x [Joystick Top](https://www.amazon.com/Joystick-Thumbsticks-Replacement-Controller-Accessories/dp/B0C3GHDLDW/ref=sr_1_8?crid=3BU4KW9HCZLRR&dib=eyJ2IjoiMSJ9.hmEPggSrayPFENEiHrSm5XMagv_Ey41rviud2-7WzUwsiC3xj3bO7n88bt379pFo3HGeRkyUNj5hJLkTcviJlDueJWsneJJl1vNZsGdrnUrdeiNCRMVZztcpepnQWZpJshooUFwg1VHSbEGEYO8InhafIf2p0FL70L410jkDBgkaDbmj-QGr1vlqjEvi2r-LksKWQ-BEQgZ75LFXpYGSpHBkQiezesehIT034mbtjvw.Race72X_BDj9CI-0kGTXh-c8DWURifn9pNEvIkygRF0&dib_tag=se&keywords=joystick+plastic&qid=1729963973&sprefix=joystick+plastick%2Caps%2C150&sr=8-8).
* 4x [JST-XH Connectors](). One 3 pin (male, female) and one 4 pin (male, female) sets.
* [M2 Screws](https://www.amazon.com/your-orders/pop?ref=ppx_yo2dv_mob_b_pop__np_0_pp&orderId=114-8647069-4298634&lineItemId=jgsotpqpmjnsoups&shipmentId=GkJxVSkZ8&packageId=1&asin=B0B93G1H9L).
* (Optional): [Cable Sleeves](https://www.amazon.com/HERCULES-Paracord-Parachute-Black-Survival/dp/B0BD2J2DQV/ref=sr_1_9?crid=1IZN8U3271YSA&dib=eyJ2IjoiMSJ9.DJ1M9JBshmhyWnkKDqiGbRBnV9cit0cVdYRV3ALZbHBNp-ZS4NB3E6pHPcTBMIIKM5958UwUQGA37yJd9m64zb3TV7_WNADZkvij0UrRQzSgP5p9jYEyhH1um_txJjtL82mQe13NP_m9Gj5nz2c6tOA-Li_VxNpWgTyBEeIxRxS2Y3ztmbvLO5qX5vW0A5vsX3cx37NFRj25xlic2WiKtlzFBme7cabkXzOzt7c1iVrVDLtGImUmeY5w9E6NQfhbxXtoAtaLaUkUXlZm8A36Qro8NtexHkqWq2HUxGe7yBM.KF24eSUxjL8StfepP6aDEpyY9BYlwgExjSKpx6m_XOg&dib_tag=se&keywords=paracord&qid=1729963341&sprefix=paracor%2Caps%2C168&sr=8-9).

## Battery Upgrade
* 1x [Battery](https://www.amazon.com/gp/product/B078TRCNRY/ref=ox_sc_act_title_2?smid=AQ7920MIUJ04Q&psc=1).
* 1x [Charge Board](https://www.amazon.com/HiLetgo-Lithium-Battery-Charging-Protect/dp/B00LTQU2RK/ref=sr_1_4?crid=OROY54GNM43Q&dib=eyJ2IjoiMSJ9.J8XGWBajjJiBRp1o_RZ4yw5Vf3b-tK07qNYodseyXKDd8ONwuvnQ6pz7yyWHrZOyks3eO-p97UB0LqqH1eLQ4ccVuGZEUdALlYaVRUZ87Vs5E2Igwk-G5F7ry-qq7L0CquQc_wu1eLGQEuekCeTgfGEic5cdeFrVj-fYy9HFextQzuuhcgVcn_A4XJ4DQimIYyMVEXEdr-Rtz9y0trt63ivgPN6CshkGUtt2c8sDO3w.NzEbURVn7PPow5-nuaxvRNvlvvHQfc0ZHJTPxuQRUto&dib_tag=se&keywords=tp4056&qid=1729963686&sprefix=tp4056%2Caps%2C166&sr=8-4).
* 2x [JST-XH Connectors](). One 4 pin (male, female) set.
* 2x [Buttons](https://www.amazon.com/Gebildet-250VAC-Prewired-Momentary-Railway/dp/B083JWJPW5/ref=sr_1_3?crid=223KZWP4V3LLQ&dib=eyJ2IjoiMSJ9.mM1hL2Wa0yy1Rlv0aRKX5FbPO8WPTWKv06JMHIpE3OJNnTvdOLzV5Gkg9cVOui3rp1QTcybEymAwUTdC03fT2D0vQNUJTSgA41VYBMr69o96SHkhyVTbHqzW7LN84rfPi0NE-qUY4l526XMdWISBBU_Sb1AmM8Vt0QQUHW4HBGKKjSEvv0evFrQSGuiXPd7pMAAvrMqv0x2jsmw6efmao3vVEzZCJixA2i8eaIoZYhQ.ZfXumC9jycFyjfJcdhznyuiRwmexbJNdo1fIgkt4X9w&dib_tag=se&keywords=buttons+electrical&qid=1729963811&sprefix=buttons+electrical%2Caps%2C152&sr=8-3). 
* [M2 Screws](https://www.amazon.com/your-orders/pop?ref=ppx_yo2dv_mob_b_pop__np_0_pp&orderId=114-8647069-4298634&lineItemId=jgsotpqpmjnsoups&shipmentId=GkJxVSkZ8&packageId=1&asin=B0B93G1H9L).
* (Optional): [Boost Converter](https://www.amazon.com/gp/product/B07L76KLRY/ref=ox_sc_act_title_3?smid=A1IU4FC9F8Y7P1&psc=1)
* (Optional): [Cable Sleeves](https://www.amazon.com/HERCULES-Paracord-Parachute-Black-Survival/dp/B0BD2J2DQV/ref=sr_1_9?crid=1IZN8U3271YSA&dib=eyJ2IjoiMSJ9.DJ1M9JBshmhyWnkKDqiGbRBnV9cit0cVdYRV3ALZbHBNp-ZS4NB3E6pHPcTBMIIKM5958UwUQGA37yJd9m64zb3TV7_WNADZkvij0UrRQzSgP5p9jYEyhH1um_txJjtL82mQe13NP_m9Gj5nz2c6tOA-Li_VxNpWgTyBEeIxRxS2Y3ztmbvLO5qX5vW0A5vsX3cx37NFRj25xlic2WiKtlzFBme7cabkXzOzt7c1iVrVDLtGImUmeY5w9E6NQfhbxXtoAtaLaUkUXlZm8A36Qro8NtexHkqWq2HUxGe7yBM.KF24eSUxjL8StfepP6aDEpyY9BYlwgExjSKpx6m_XOg&dib_tag=se&keywords=paracord&qid=1729963341&sprefix=paracor%2Caps%2C168&sr=8-9).