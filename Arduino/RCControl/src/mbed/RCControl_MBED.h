#ifndef RCControl_MBED_h
#define RCControl_MBED_h

#include "Arduino.h"
#include <ArduinoBLE.h>

class RCControl_BLE
{
public:
	RCControl_BLE(const char* Service_UUID, const char* Chara_UUID, String name, int ByteLength = 64);
	void Begin();
	void loop();

	float JoyStickX;
	float JoyStickY;
	float GyroA;
	float GyroB;
	float GyroG;

	bool connected;

	bool JoystickUpdate();
	bool GyroUpdate();

	void ParseMessage(String msg);

	String Dequeue();
	bool Enqueue(String data);
	bool Full();
	bool Empty();
private:
	static RCControl_BLE* _instance;

	BLEService _serv;
	BLECharacteristic _char;
	String _name;

	BLEDevice central;

	int _NumOfItems;
	int _frontPointer;
	int _backPointer;
	String _Queue[16];

	bool JoystickUpdated;
	bool GyroUpdated;

	void HandleOnConnect(BLEDevice central);
	void HandleOnDisconnect(BLEDevice central);
	void HandleOnWrite(BLEDevice central, BLECharacteristic characteristic);

	static void onConnect(BLEDevice central);
	static void onDisconnect(BLEDevice central);
	static void onWrite(BLEDevice central, BLECharacteristic characteristic);
};

#endif