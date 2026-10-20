[Arduino instructions](ARDUINO.md)  
[Raspberry Pi instructions](RASPBERRY_PI.md)

# Robot Competition Controller

This is a custom made library and wireless controller accessible through a browser, designed to be compatable for every team in the University of York Robot Competition. This library supports Arduino Nano 33 BLE rev2, the Arduino Nano ESP32 and the Raspberry Pi Pico 2 W. If you stumbled upon this repo and you're not part of the UoY then you're unlikely to find use out of this controller.  

This library supports two forms of communication, Bluetooth Low Energy (BLE) and WiFi (making use of websockets). The Arduino Nano 33 BLE rev2 only has BLE, while the Arduino Nano ESP32 and the Raspberry Pi Pico 2 W supports both BLE and WiFi. See the relevant sections for instructions on how to use the two different communication mediums.

To make the WiFi communication medium possible, I used a stripped down version of Marcus Sattler's websocket library. The original library can be found [here](https://github.com/Links2004/arduinoWebSockets)
  
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

# How to use website

Having the website connect to the Arduino / Pico is slightly different depending on which communication medium you are using

## BLE communication medium

This applies to both the Arduino Nano 33 BLE rev2, the Arduino Nano ESP32, and the Raspberry Pi Pico 2 W.

1. Connect your microcontroller to power
2. Ensure Bluetooth is enabled on your device
3. On the website, press the `Settings` icon and configure the UUIDs to match the UUIDs on the Arduino
4. Then press `Connect` A small window will appear, when your Arduino shows up in the list select it and connect to it.
5. After a moment the website should show the connection successful, move the joystick around and you should see the x and y value on your serial monitor.

### Gyroscope input

Unique to the BLE communication medium, there is functionality to use your devices gyroscope. To enable this go to settings and toggle the switch to enable the gyro, it will take your current rotation as a reference then tell you the change from that reference point. You can change this reference to any point you like by simply pressing the `Zero gyro` button.

## WiFi communication medium

This applies only to the Arduino Nano ESP32 and the Raspberry Pi Pico 2 W.

1. Connect you microcontroller to power
2. Open the WiFi settings on your phone or laptop and connect to the network with the SSID of your robot (the password is the one you specified at the top of the code). Your device might warn you and say that there is no internet connection, this is not an issue.
3. Once connected, open your browser and type `192.168.4.1` into the URL bar (this might take a moment to load).
4. On the website, press the `Settings` icon then press the `Connect` button. Once successful you can move the joystick and you should see the x and y value on your serial monitor

# Tailoring the website to your needs.

If you wish to add more features to the website such as a button that sends a string to the arduino, or a slider to adjust speed, then I encourage you pursue this.  
  
For the BLE implementation, you will need to host the website yourself, I recommend hosting the site you create through the university on your personal webspace. See details on how to set it up [here](https://www.york.ac.uk/it-services/tools/personal-web-space/).  
To make changes, head to this [link](https://www-users.york.ac.uk/~nqn524/download) and download and extract the zip file, open the html file in your editor of choice and make your changes.  
Please be aware that if you do this then any changes that I make to the website will obviously not carry over to your website.  

If you wish to send string messages to the Arduino then you can do so, on the back end of the website there is a function called 'send' (creative name I know) that is able to send any string to the connected BLE device. **Please note that you cannot have a message start with `js,` or `gyro,`, the Arduino checks if a message has either of these prefixes to know if it is a joystick, gyro or a message to the queue**. Here is an example of a button that will simply send the string `Hello` to the Arduino:  
```html
<button class="allbuttons" onclick="send('Hello')">Send Hello</button>
```  

If you are using the WiFi communication medium, you will have to open the library's source files, for the Arduino Nano ESP32 open `RCControl > src > esp32 > website > INDEX_HTML.h` and for the Raspberry Pi Pico 2 W open `lib > RCControl > website > index.html`, make any changes you wish and save. Alternatively you can completely avoid the website - the website is effectively just opening up a websocket and sending joystick updates through it. If you want to you can make a program that simply connects to a websocket, for the Arduino Nano ESP32 you can use either address `ws://esp32.local:8080` or `ws://192.168.4.1:8080`, for the Raspberry Pi Pico 2 W you must use the address `ws://192.168.4.1/ws`. If you do this your device will still need to be connected to the network exposed by the ESP32 / Pico. As an example I opened the nodejs console and ran the following code:

```javascript
> let ws = new WebSocket("ws://esp32.local:8080");
undefined
> ws.send("test");
undefined
> ws.close();
undefine
```

And this is the output form the ESP32 in the serial monitor
```
Client connected
Queue: test
Client disconnected
```

To be able to read any sent data on the Arduino, the library has a circuilar queue built in and any recieved data that is not the joystick will be placed on this queue. The queue has a max size of 16, after more than 16 strings have been recieved new ones will be discarded. The following block of code can be found in the example and shows how you are able to access this queue.
```cpp
if (!RCC.Empty()) {
  String data = RCC.Dequeue();
  Serial.println(data);
}
```

# Author and Maintainer

The Author and Maintainer of this Github, the RCContol library and the website is Karl Smirthwaite, if you need to contact me for any reason, please email me at nqn524@york.ac.uk
