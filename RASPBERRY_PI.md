# Disclaimer

These instructions are only for the Raspberry Pi Pico 2 W only. See [thses instructions](ARDUINO.md) for the Arduino Nano 33 Sense Rev2 and the Arduino Nano ESP32

# How to install the library
1. Follow [this guide](https://thepihut.com/blogs/raspberry-pi-tutorials/raspberry-pi-pico-getting-started-guide) until the end of the `Setting up Thonny` section to install the Thonny IDE and install the correct firmware onto the Raspberry Pi
2. Install the `Picozero` and `Microdot` libraries.
- Press `Tools > Manage pagacages`
- Type `Picozero`
3. Everything should now be set up.
4. There is an example you can use. You can access it by going to `lib > RCControl > example.py`, alternatively you can access it from [here](RaspberryPiPico2W/RCControl/example.py).
5. Uncomment either line 14 or line 22 dependind on which mode you want to use (**NOTE:** do not uncomment both)
6. Change the parameters:
- For BLE, uncomment line 14:  
  `RCC = RCControl_BLE("12345678-1234-1234-1234-123456789abc", "abcdef01-1234-1234-1234-123456789abc", "Pico")`
  -  `12345678-1234-1234-1234-123456789abc` represents the Service UUID, using this [UUID generator](https://www.uuidgenerator.net/) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)
  -  `abcdef01-1234-1234-1234-123456789abc` represents the Characteristic UUID, using this [UUID generator](https://www.uuidgenerator.net/) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)  
    **NOTE**: The Service UUID and Characteristic UUID MUST be different
  - `Pico` represents the name of the Arduino when broadcasting, change this to something like your team name or something similar to seperate it from other teams robots
- For WiFi, uncomment line 22:  
  `RCC = RCControl_WiFi("Pico", "12345678")`
  - `Pico` reprenets the SSID of the network served by the Raspberry Pi. Change this to something such as your team name to differentiate it from other teams robots
  - `12345678` repesents the password of the network. Change this to something and share it amongst your team, do not share it with other teams or they might be able to sabotage you.
    - If you want to go the extra step and remove the plaintext password from your source code, you could put it in an external file and have your main file open and read the password file, but this is entirely optional

**To use the controller follow the instructions in the [README.md](README.md)**
