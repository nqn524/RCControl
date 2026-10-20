# Disclaimer

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

# Hardware specific instructions

Once you have set up the library with your IDE then proceed with the following instructions based on your selected hardware.

## Arduino Nano 33 BLE rev2
 1. There is an example that you can access through the Arduino IDE, open it by going:  
`File > Examples > RCControl > mbed`  
Assuming everything is installed correctly this should work immedietly when you upload to your Arduino
 - Alternatively you can access it from the Github [here](Arduino/RCControl/examples/mbed/mbed.ino)
2. On line 3 in the example you will see the following code:  
`RCControl_BLE RCC("12345678-1234-1234-1234-123456789abc", "abcdef01-1234-1234-1234-123456789abc", "Example", 64);`  
You will need to change a few of the arguments:  
 - `12345678-1234-1234-1234-123456789abc` represents the Service UUID, using this [UUID generator](https://www.uuidgenerator.net) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)
 - `abcdef01-1234-1234-1234-123456789abc` represents the Characteristic UUID, using this [UUID generator](https://www.uuidgenerator.net) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)  
 **NOTE: The Service UUID and Characteristic UUID MUST be different**
 - `Example` represents the name of the Arduino when broadcasting, change this to something like your team name or something similar to seperate it from other teams robots
 - `64` repesents the maximum byte length of received data, if you start sending large strings of data they will get cut off after 64 characters, you can increase this value up to 512 if you need to

### Disclaimer when using the IMU

The Arduino Nano 33 BLE rev2 has an on board IMU that allows you to get the boards linear acceleration, angular acceleration and magnetic field strength in all three axis. If you plan on using it then make sure to use the library called `Arduino_BMI270_BMM150` and **NOT** `Arduino_LSM9DS1`. A lot of online documentation (including official Arduino documentation) says to use the wrong library. Thankfully the syntax between the two libraries is identical, the only difference between them is the model of IMU they are compatible with. The IMU on the Arduino Nano 33 BLE sense Rev2 is made up of the 3-axis accelerometer and gyroscope `BMI270`, and the 3-axis magnetometer `BMM150`.

## Arduino Nano ESP32

1. There are a couple examples that you can access through the Arduino IDE, you can open them by going:  
`File > Examples > RCControl > esp32 > BLE or WiFi`  
Assuming everything is installed correctly this should work immedietly when you upload to you Arduino.
 - Alternatively you can find the examples from the GitHub. [BLE](RCControl/examples/esp32/BLE/BLE.ino) or [WiFi](RCControl/examples/esp32/WiFi/WiFi.ino)
2. You will need to change line 3 in the example on both of the examples.
 - For the BLE example, you will see this line:  
   `RCControl_BLE RCC("12345678-1234-1234-1234-123456789abc", "abcdef01-1234-1234-1234-123456789abc", "Example");`
    - `12345678-1234-1234-1234-123456789abc` represents the Service UUID, using this [UUID generator](https://www.uuidgenerator.net) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)
    - `abcdef01-1234-1234-1234-123456789abc` represents the Characteristic UUID, using this [UUID generator](https://www.uuidgenerator.net) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)  
 **NOTE: The Service UUID and Characteristic UUID MUST be different**
    - `Example` represents the name of the Arduino when broadcasting, change this to something like your team name or something similar to seperate it from other teams robots
 - For the WiFi example, you will see this line:  
   `RCControl_WiFi RCC("Example", "password");`
    - `Example` represents the SSID of the network that the ESP32 will expose, change this to something like your team name or something similar to seperate it from other teams robots.
    - `password` represents the password of the network that the ESP32 will expose, change this to something and share it amongst your team to prevent other teams from connecting to your robot and sabotaging you.
