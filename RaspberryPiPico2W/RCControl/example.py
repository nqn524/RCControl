from RCControl import RCControl_BLE, RCControl_WiFi

RCC = None

def main():
    global RCC
    
    '''
    Bluetooth version
    Compatible with Android and Laptops. Does NOT work with IOS (untested on Mac)
    Access controller from this url:
        https://www-users.york.ac.uk/~nqn524/controller
    '''
    #RCC = RCControl_BLE("12345678-1234-1234-1234-123456789abc", "abcdef01-1234-1234-1234-123456789abc", "Pico")
    
    '''
    WiFi / Websocket version
    Compatible with all devices
    Access controller from this url:
        http://192.168.4.1
    '''
    #RCC = RCControl_WiFi("Pico", "12345678")

def loop():
    if (RCC.connected):
        if RCC.JoystickUpdate():
          print(f"{RCC.JoyStickX}   {RCC.JoyStickY}")  

        if RCC.GyroUpdate():
          print(f"{RCC.GyroA}   {RCC.GyroB}   {RCC.GyroG}")

        if not RCC.Empty():
          print(f"Queue:  {RCC.Dequeue()}")
                         
    
main()

while True:
    loop()
