#if defined(ARDUINO_ARCH_MBED)

#include "Arduino.h"
#include <ArduinoBLE.h>
#include "RCControl_MBED.h"


RCControl_BLE* RCControl_BLE::_instance = nullptr;

RCControl_BLE::RCControl_BLE(const char* Service_UUID, const char* Chara_UUID, String name, int ByteLength) :
    _serv(Service_UUID),
    _char(Chara_UUID, BLEWrite, ByteLength)
{
    _name = name;
}

void RCControl_BLE::Begin() {
    if (!BLE.begin()) {
        Serial.println("Failed starting BLE!");
        while (true);
    }

    _instance = this;
    String fullName = String("RC-Car ") + _name;
    BLE.setLocalName(fullName.c_str());
    BLE.setAdvertisedService(_serv);
    _serv.addCharacteristic(_char);
    BLE.addService(_serv);

    BLE.setEventHandler(BLEConnected, onConnect);
    BLE.setEventHandler(BLEDisconnected, onDisconnect);
    _char.setEventHandler(BLEWritten, onWrite);

    BLE.advertise();
}

bool RCControl_BLE::JoystickUpdate() {
    if (JoystickUpdated) {
        JoystickUpdated = false;
        return true;
    }
    else {
        return false;
    }
}

bool RCControl_BLE::GyroUpdate() {
    if (GyroUpdated) {
        GyroUpdated = false;
        return true;
    }
    else {
        return false;
    }
}

void RCControl_BLE::ParseMessage(String msg) {
    JoystickUpdated = false;
    GyroUpdated = false;

    int length = msg.length();
    String raw = msg;

    String data = "";
    for (int i = 0; i < length; i++) {
        data += (char)raw[i];
    }

    if (data.startsWith("js,")) {
        JoystickUpdated = true;

        data = data.substring(3);

        String splitData[2];
        int index = 0;
        String cur;
        for (int i = 0; i < data.length(); i++) {
            if (data.substring(i, i + 1) == ",")
            {
                splitData[index] = cur;
                cur = "";
                index += 1;
            }
            else
            {
                cur = cur + data[i];
            }
        }

        splitData[index] = cur;

        JoyStickX = (splitData[0].toFloat()) / 128 - 1;
        JoyStickY = (splitData[1].toFloat()) / 128 - 1;

        if (abs(JoyStickX) <= 0.05) {
            JoyStickX = 0;
        }
        if (abs(JoyStickY) <= 0.05) {
            JoyStickY = 0;
        }
    }
    else if (data.startsWith("gyro,")) {
        GyroUpdated = true;

        data = data.substring(5);

        String splitData[3];
        int index = 0;
        String cur;
        for (int i = 0; i < data.length(); i++) {
            if (data.substring(i, i + 1) == ",")
            {
                splitData[index] = cur;
                cur = "";
                index += 1;
            }
            else
            {
                cur = cur + data[i];
            }
        }

        splitData[index] = cur;

        GyroA = splitData[0].toFloat();
        GyroB = splitData[1].toFloat();
        GyroG = splitData[2].toFloat();
    }
    else {
        Enqueue(data);
    }
}

void RCControl_BLE::HandleOnWrite(BLEDevice central, BLECharacteristic characteristic) {
    int length = characteristic.valueLength();
    const uint8_t* raw = characteristic.value();

    String data = "";
    for (int i = 0; i < length; i++) {
        data += (char)raw[i];
    }

    ParseMessage(data);
}

void RCControl_BLE::HandleOnConnect(BLEDevice central) {
    Serial.println("Client connected");
    connected = true;
}

void RCControl_BLE::HandleOnDisconnect(BLEDevice central) {
    Serial.println("Client disconnected");
    connected = false;
}



void RCControl_BLE::onWrite(BLEDevice central, BLECharacteristic characteristic) {
    if (_instance) {
        _instance->HandleOnWrite(central, characteristic);
    }
}

void RCControl_BLE::onConnect(BLEDevice central) {
    if (_instance) {
        _instance->HandleOnConnect(central);
    }
}

void RCControl_BLE::onDisconnect(BLEDevice central) {
    if (_instance) {
        _instance->HandleOnDisconnect(central);
    }
}

String RCControl_BLE::Dequeue() {
    if (!Empty()) {
        String DataToReturn = _Queue[_frontPointer];

        _frontPointer += 1;

        if (_frontPointer >= 16) {
            _frontPointer = 0;
        }

        _NumOfItems -= 1;

        return DataToReturn;
    }
    else {
        return "";
    }
}

bool RCControl_BLE::Enqueue(String data) {
    if (!Full()) {
        _Queue[_backPointer] = data;
        _backPointer += 1;
        if (_backPointer >= 16) {
            _backPointer = 0;
        }

        _NumOfItems += 1;

        return true;
    }
    else { return false; }
}

bool RCControl_BLE::Full() {
    return _NumOfItems == 16;
}

bool RCControl_BLE::Empty() {
    return _NumOfItems == 0;
}

#endif