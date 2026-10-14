These instructions are for the Arduino Nano 33 Sense Rev2 and the Arduino Nano ESP32 only. See [these instructions](RASPBERRY_PI.md) for the Raspberry Pi Pico 2 W.

# How to install the library

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software/)
2. Skip this step if you plan on using the WiFi medium. Install the `ArduinoBLE` library through the Arduino IDE library manager
    - You will be asked if you want to install `Arduino_SpiNINA` library, select `Install all`
3. Downlaod and extract the zip file from the Github's [releases](https://github.com/nqn524/RCControl/releases/latest) page
4. Copy the `RCContol` folder into `Documents > Arduino > libraries`. The file structre should look like the following:  

```
└── Arduino/  
  └── libraries/  
    ├── Arduino_SpiNINA/
    ├── ArduinoBLE/  
    └── RCControl/  
      ├── examples/  
      ├── src/  
      ├── Keywords.txt  
      └── library.properties
```
- If you skipped step 2 then you won't have either `Arduino_SpiNINA` or `ArduinoBLE` in you folder, this is fine.
5. You will now want to restart the Arduino IDE to allow it to recognise the RCContol library
6. Everything should now be setup.
