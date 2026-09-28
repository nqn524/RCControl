# Robot Competition Controller

This is a custom made library and wireless controller accessible through a browser, designed to be compatable for every team in the University of York Robot Competition. This library supports Arduino Nano 33 BLE rev2 and the Arduino Nano ESP32. If you stumbled upon this repo and you're not part of the UoY then you're unlikely to find use out of this controller.  

This library supports two forms of communication, Bluetooth Low Energy (BLE) and WiFi (making use of websockets). The Arduino Nano 33 BLE rev2 only has BLE, while the Arduino Nano ESP32 supports both BLE and WiFi. See the relevant sections for instructions on how to use the two different communication mediums.
  
The controller is accessible from this link https://www-users.york.ac.uk/~nqn524

# Disclaimer
To be able to use this on IOS (untested on Macbook) you will have to use the ESP32 WiFi medium.  
If you are planning on using the Bluetooth approach then be aware that only a few browsers have BLE compatability.  

| Chrome | Edge | Firefox | Safari | Opera | Opera<br>mini | Internet<br>explorer | Samsung<br>internet | Brave |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/chrome.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/edge.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/firefox.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/safari.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/opera.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/opera_mini.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/ie.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/samsung_internet.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/brave.png" width="40"/> |
| &#x2705; | &#x2705; | &#x274C; | &#x274C; | &#x2705; | &#x274C; | &#x274C; | &#x2705; | &#x274C; |

**Note: This information is accurate as of 28/09/2026**  
This information is from [caniuse.com](https://caniuse.com/web-bluetooth)

# How to install the library

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software/)
2. Skip this step if you plan on using the WiFi medium. Install the `ArduinoBLE` library through the Arduino IDE library manager
    - You will be asked if you want to install `Arduino_SpiNINA` library, select `Install all`
3. Downlaod and extract the zip file from the Github's [releases](releases) page
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
 - Alternatively you can access it from the Github [here](RCControl/examples/mbed/mbed.ino)
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

# How to use website

Having the website connect to the Arduino is slightly different depending on which communication medium you are using

## BLE communication medium

This applies to both the Arduino Nano 33 BLE rev2 and the Arduino Nano ESP32.

1. Connect your Arduino to power
2. Ensure Bluetooth is enabled on your device
3. On the website, press the `Settings` icon and configure the UUIDs to match the UUIDs on the Arduino
4. Then press `Connect` A small window will appear, when your Arduino shows up in the list select it and connect to it.
5. After a moment the website should show the connection successful, move the joystick around and you should see the x and y value on your serial monitor.

## WiFi communication medium

This applies only to the Arduino Nano ESP32.

1. Connect you Arduino to power
2. Open the WiFi settings on your phone or laptop and connect to the network with the SSID of your robot (the password is the one you specified at the top of the code). Your device might warn you and say that there is no internet connection, this is not an issue.
3. Once connected, open your browser and type `esp32.local` into the URL bar (this might take a moment to load).
4. On the website, press the `Settings` icon then press the `Connect` button. Once successful you can move the joystick and you should see the x and y value on your serial monitor

# Tailoring the website to your needs.

If you wish to add more features to the website such as a button that sends a string to the arduino, or a slider to adjust speed, then I encourage you pursue this. You will need a laptop or a computer to do this, so to use it on your phone I recommend hosting the site you create on your personal webspace. See details on how to set it up [here](https://www.york.ac.uk/it-services/tools/personal-web-space/).  
To make changes you will have to navigate to the website and press `Ctrl+S` this will download the html file of the web app to your device, open the html file in your editor of choice and make your changes.  
Please be aware that if you do this then any changes that I make to the website will obviously not carry over to your website.  
If you wish to send string messages to the Arduino then you can do so, on the back end of the website there is a function called 'send' (creative name I know) that is able to send any string to the connected BLE device. **Please note that you cannot have a message start with `js,` or `gyro,`, the Arduino checks if a message has either of these prefixes to know if it is a joystick, gyro or a message to the queue**. Here is an example of a button that will simply send the string `Hello` to the Arduino:  
```html
<button class="allbuttons" onclick="send('Hello')">Send Hello</button>
```

If you are using the WiFi communication medium with the ESP32, if you wish you can completely avoid the website - the website is effectively just opening up a websocket and sending joystick updates through it. If you want to you can make a program that simply connects to a websocket on the following address `ws://esp32.local:8080` or `ws://192.168.4.1:8080`. If you do this your device will still need to be connected to the network exposed by the ESP32. As an example I opened the nodejs console and ran the following code:

To be able to read any sent data on the Arduino, the library has a circuilar queue built in and any recieved data that is not the joystick will be placed on this queue. The queue has a max size of 16, after more than 16 strings have been recieved new ones will be discarded. The following block of code can be found in the example and shows how you are able to access this queue.
```cpp
if (!RCC.Empty()) {
  String data = RCC.Dequeue();
  Serial.println(data);
}
```

# Author and Maintainer

The Author and Maintainer of this Github, the RCContol library and the website is Karl Smirthwaite, if you need to contact me for any reason, please email me at nqn524@york.ac.uk
