#if defined(ARDUINO_ARCH_ESP32)

#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "RCControl_ESP32.h"

class CharCallbacks : public BLECharacteristicCallbacks {
    RCControl_BLE* _RCC;

public:
    CharCallbacks(RCControl_BLE* RCC) {
        _RCC = RCC;
    }

private:
    void onWrite(BLECharacteristic* characteristic) override {
        String message = String(characteristic->getValue().c_str());

        _RCC->ParseMessage(message);
    }
};

class ServerCallbacks : public BLEServerCallbacks {
    RCControl_BLE* _RCC;

public:
    ServerCallbacks(RCControl_BLE* RCC) {
        _RCC = RCC;
    }

private:
    void onConnect(BLEServer* server) override {
        Serial.println("Client connected");
        _RCC->connected = true;
    }
    void onDisconnect(BLEServer* server) override {
        Serial.println("Client disconnected");
        _RCC->connected = false;

        server->startAdvertising();
    }
};

/// <summary>
/// Base
/// </summary>
RCControl::RCControl() { }
void RCControl::Begin() { }

bool RCControl::JoystickUpdate() {
    if (JoystickUpdated) {
        JoystickUpdated = false;
        return true;
    }
    else {
        return false;
    }
}

bool RCControl::GyroUpdate() {
    if (GryoUpdated) {
        GryoUpdated = false;
        return true;
    }
    else {
        return false;
    }
}

void RCControl::ParseMessage(String msg) {
    JoystickUpdated = false;
    GryoUpdated = false;

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
        GryoUpdated = true;

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

String RCControl::Dequeue() {
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

bool RCControl::Enqueue(String data) {
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

bool RCControl::Full() {
    return _NumOfItems == 16;
}

bool RCControl::Empty() {
    return _NumOfItems == 0;
}


/// <summary>
/// Chessburger :3
/// </summary>
/// <param name="Service_UUID">Service UUID</param>
/// <param name="Chara_UUID">Characteristic UUID</param>
/// <param name="name">The name that will be broadcast</param>
/// 
RCControl_BLE::RCControl_BLE(const char* Service_UUID, const char* Chara_UUID, String name)
{
    SERVICE_UUID = Service_UUID;
    CHAR_UUID = Chara_UUID;
    _name = name;
}

void RCControl_BLE::Begin() {
    BLEDevice _dev = BLEDevice();
    _dev.init((String("RC-Car ") + _name).c_str());

    BLEServer* _server = _dev.createServer();
    BLEService* _serv = _server->createService(SERVICE_UUID);
    BLECharacteristic* _char = _serv->createCharacteristic(CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);

    _char->setCallbacks(new CharCallbacks(this));
    _server->setCallbacks(new ServerCallbacks(this));

    _serv->start();

    _dev.getAdvertising()->addServiceUUID(SERVICE_UUID);

    _dev.startAdvertising();

}



RCControl_WiFi::RCControl_WiFi() {

}

void RCControl_WiFi::Begin() {

}

#endif