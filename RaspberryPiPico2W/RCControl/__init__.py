from micropython import const

import bluetooth
import network

from microdot.microdot import Microdot
from microdot.websocket import with_websocket
import _thread

_IRQ_CENTRAL_CONNECT = const(1)
_IRQ_CENTRAL_DISCONNECT = const(2)
_IRQ_GATTS_WRITE = const(3)

class RCControl:
    def __init__(self):
        self._NumOfItems = 0
        self._frontPointer = 0;
        self._backPointer = 0;
        self._Queue = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
        
        self.JoystickUpdated = False
        self.GryoUpdated = False
        self.connected = False
        
        self.JoyStickX = 0
        self.JoyStickY = 0
        self.GyroA = 0
        self.GyroB = 0
        self.GyroG = 0
        
        
    def ParseMessage(self, msg):
        self.JoystickUpdated = False
        self.GryoUpdated = False
        
        if msg.startswith("js,"):
            msg = msg[3:]
            
            splitData = msg.split(",")
            
            self.JoyStickX = float(splitData[0]) / 128 - 1
            self.JoyStickY = float(splitData[1]) / 128 - 1
            
            if abs(self.JoyStickX) <= 0.05:
                self.JoyStickX = 0
            
            if abs(self.JoyStickY) <= 0.05:
                self.JoyStickY = 0
    
            self.JoystickUpdated = True
        elif msg.startswith("gyro,"):
            msg = msg[5:]
            
            splitData = msg.split(",")
            
            print(msg)
            print(splitData)
            
            self.GyroA = float(splitData[0])
            self.GyroB = float(splitData[1])
            self.GyroG = float(splitData[2])
            self.GyroUpdated = True
        else:
            self.Enqueue(msg)
            
    
    def JoystickUpdate(self):
        if self.JoystickUpdated:
            self.JoystickUpdated = False
            return True
        else:
            return False
        
    def GyroUpdate(self):
        if self.GryoUpdated:
            self.GryoUpdated = False
            return True
        else:
            return False
    
    def Dequeue(self):
        if not self.Empty():
            DataToReturn = self._Queue[self._frontPointer]
            
            self._frontPointer += 1
            
            if self._frontPointer >= 16:
                self._frontPointer = 0
            
            self._NumOfItems -= 1
            
            return DataToReturn
    def Enqueue(self, data):
        if not self.Full():
            self._Queue[self._backPointer] = data
            
            self._backPointer += 1
            
            if self._backPointer >= 16:
                self._backPointer = 0
            
            self._NumOfItems += 1
            
            return True
    def Full(self):
        return self._NumOfItems == 16
    def Empty(self):
        return self._NumOfItems == 0

class RCControl_WiFi(RCControl):
    def __init__(self, SSID, password):        
        self.SSID = SSID
        self.password = password
        
        self.ap = network.WLAN(network.AP_IF)
        self.ap.config(essid=self.SSID, password=self.password)
        self.ap.active(True)
        
        self.server = Microdot()

class RCControl_BLE(RCControl):
    def __init__(self, Service_UUID, Chara_UUID, name):
        super().__init__()
        
        print("Starting BLE")
        
        self.SERVICE_UUID = bluetooth.UUID(Service_UUID)
        self.CHARA_UUID = bluetooth.UUID(Chara_UUID)
        self.name = "RC-Car " + name
        
        self.ble = bluetooth.BLE()

        self.ble.active(True)
        self.ble.irq(self.irq)
        self.ble.config(mtu=64)

        ((self.char_handle,),) = self.ble.gatts_register_services(
            (
                (
                    self.SERVICE_UUID,
                    (
                        (
                            self.CHARA_UUID, (bluetooth.FLAG_WRITE | bluetooth.FLAG_WRITE_NO_RESPONSE)
                            )
                        ,)
                    ,)
                ,)
            )

        self.advertise()

    def advertise(self):
        payload = bytearray()
        payload += bytes([2, 0x01, 0x06])
    
        name_bytes = self.name.encode()

        payload += bytes([len(name_bytes) + 1, 0x09])
        payload += name_bytes

        self.ble.gap_advertise(100000, adv_data=payload)

    def irq(self, event, data):
        if event == _IRQ_CENTRAL_CONNECT:
            print("Device connected")
            self.connected = True
        elif event == _IRQ_CENTRAL_DISCONNECT:
            print("Device disconnected")
            self.connected = False
            self.advertise()
        elif event == _IRQ_GATTS_WRITE:
            conn_handle, value_handle = data

            if value_handle == self.char_handle:
                value = self.ble.gatts_read(self.char_handle)

                message = value.decode("utf-8")
                RCControl.ParseMessage(self, message)
                
                
class RCControl_WiFi(RCControl):
    def __init__(self, ssid, passwodd):
        super().__init__()
        
        print("Starting WiFi")
        
        self.ap = network.WLAN(network.AP_IF)
        self.ap.active(True)
        self.ap.config(essid=ssid, password=passwodd)
        
        self.app = Microdot()
        
        @self.app.route("/ws")
        @with_websocket
        async def websocket(request, ws):
            print("Websocket connected")
            self.connected = True

            try:
                while True:
                    message = await ws.receive()

                    if message is None:
                        break

                    self.ParseMessage(message)

            except Exception as e:
                print("Websocket error:", e)

            finally:
                self.connected = False
                print("Websocket disconnected")
                
        @self.app.route("/joystick.js")
        def joystick(request):
            js = ""
            with open("/lib/RCControl/website/joystick.js", "r") as f:
                js = f.read()
            return js, 200, {'Content-Type': 'application/javascript'}

        @self.app.route('/')
        def index(request):
            html = ""
            with open("/lib/RCControl/website/index.html", "r") as f:
                html = f.read()
            
            return html, 200, {'Content-Type': 'text/html'}

        _thread.start_new_thread(self._run_server, ())
        
    def _run_server(self):
        self.app.run(port=80)


if __name__ == "__main__":
    raise Exception("You cannot run this code")