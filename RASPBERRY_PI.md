# Disclaimer

These instructions are only for the Raspberry Pi Pico 2 W only. See [thses instructions](ARDUINO.md) for the Arduino Nano 33 Sense Rev2 and the Arduino Nano ESP32

# How to install the library
1. Download and install the [Thonny IDE](https://thonny.org/)
2. Change the interpreter to micropython for Raspberry Pi Pico
- Insert your Raspberry Pi if you have not yet done so
- Press `Run > Configure interpreter`
- Set the interpreter to `Micropython (Raspberry Pi Pico)`
3. Install the `Picozero` and `Microdot` libraries.
- Press `Tools > Manage pagacages`
- Type `Picozero`
4. There is an example you can use. You can access it by going to `lib > RCControl > example.py`, alternatively you can access it from [here](RaspberryPiPico2W/RCControl/example.py).
5. Uncomment either line 14 or line 22 dependind on which mode you want to use (**NOTE:** do not uncomment both)
6. Change the parameters:
- For BLE:
  -  sdf
