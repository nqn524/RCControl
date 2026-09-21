# Robot Competition Controller

This is a custom made library and wireless controller accessible through a browser, designed to be compatable for every team in the Univerty of York Robot Competition. This will only work on the Arduino Nano 33 BLE sense Rev2 that every team has been provided. If you stumbled upon this repo and you're not part of the UoY then you're unlikekly to find use out of this controller.  
  
The controller is accessible from this link https://www-users.york.ac.uk/~nqn524

# Disclaimer
The webapp is not compatable with IOS devices due to Apple's higher security concerns, limiting websites from accessing Bluetooth devices.  
Also note, only a select few browsers have Bluetooth compatibility, see the table below to find out which can and cannot.  

| Chrome | Edge | Firefox | Safari | Opera | Opera<br>mini | Internet<br>explorer | Samsung<br>internet | Brave |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/chrome.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/edge.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/firefox.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/safari.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/opera.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/opera_mini.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/ie.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/samsung_internet.png" width="40"/> | <img src="https://www-users.york.ac.uk/~nqn524/BrowserIcons/brave.png" width="40"/> |
| &#x2705; | &#x2705; | &#x274C; | &#x274C; | &#x2705; | &#x274C; | &#x274C; | &#x2705; | &#x274C; |

**Note: This information is accurate as of 19/05/2026**  
This information is from [caniuse.com](https://caniuse.com/web-bluetooth)

# How to install the library

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software/)
2. Install the `ArduinoBLE` library through the Arduino IDE library manager
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

5. You will now want to restart the Arduino IDE to allow it to recognise the RCContol library
6. Everything should now be setup. 
 - There is an example that you can access through the Arduino IDE, open it by going:  
`File > Examples > RobotCompControl > ExampleMotorControl`  
Assuming everything is installed correctly this should work immedietly when you upload to your Arduino
 - Alternatively you can access it from the Github [here](RCControl/examples/ExampleMotorControl/ExampleMotorControl.ino)
7. On line 7 in the example you will see the following code:  
`RCControl RCC("12345678-1234-1234-1234-123456789abc", "abcdef01-1234-1234-1234-123456789abc", "Example", 64);`  
You will need to change a few of the arguments:  
 - `12345678-1234-1234-1234-123456789abc` represents the Service UUID, using this [UUID generator](https://www.uuidgenerator.net) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)
 - `abcdef01-1234-1234-1234-123456789abc` represents the Characteristic UUID, using this [UUID generator](https://www.uuidgenerator.net) generate and replace the template UUID (Keep hold of this UUID, you will need it on the website when you come to connecting to the Arduino)  
 **NOTE: The Service UUID and Characteristic UUID MUST be different**
 - `Example` represents the name of the Arduino when broadcasting, change this to something like your team name or something similar to seperate it from other teams robots
 - `64` repesents the maximum byte length of received data, if you start sending large strings of data they will get cut off after 64 characters, you can increase this value up to 512 if you need to

# Disclaimer when using the IMU

The Arduino that we are using has an on board IMU that allows you to get the boards linear acceleration, angular acceleration and magnetic field strength in all three axis. If you plan on using it then make sure to use the library called `Arduino_BMI270_BMM150` and **NOT** `Arduino_LSM9DS1`. A lot of online documentation (including official Arduino documentation) says to use the wrong library. Thankfully the syntax between the two libraries is identical, the only difference between them is the model of IMU they are compatible with. The IMU on the Arduino Nano 33 BLE sense Rev2 is made up of the 3-axis accelerometer and gyroscope `BMI270`, and the 3-axis magnetometer `BMM150`.

# How to use website

1. Connect your Arduino to power
2. Ensure Bluetooth is enabled on your device
3. On the website, press `Settings` and configure the UUIDs to match the UUIDs on the Arduino
4. Then press `Connect` A small window will appear, when your Arduino shows up in the list select it and connect to it.
5. After a moment the website should show the connection successful, move the joystick around and it should start moving the motors.

# Making changes to the website

If you wish to add more features to the website such as a button that sends a string to the arduino, or a slider to adjust speed, then I encourage you pursue this.  
To make changes you will have to navigate to the website and press `Ctrl+S` this will download the html file of the web app to your device, open the html file in your editor of choice and make your changes.  
Please be aware that if you do this then any changes that I make to the website will obviously not carry over to your website.  

  If you wish to send string messages to the arduino then you can do so, on the back end of the website there is a function called 'send' (creative name I know) that is able to send any string to the connected BLE device. To be able to read this sent data on the Arduino, the library has a circuilar queue built in and any recieved data that is not the joystick will be placed on this queue. The queue has a max size of 16, after more than 16 strings have been recieved new ones will be discarded. The following block of code can be found in the example and shows how you are able to access this queue.
```cpp
if (!RCC.Empty()) {
  String data = RCC.Dequeue();
  Serial.println(data);
}
```

# Author and Maintainer

The Author and Maintainer of this Github, the RCContol library and the website is Karl Smirthwaite, if you need to contact me for any reason, please email me at nqn524@york.ac.uk
