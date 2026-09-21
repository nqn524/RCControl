#ifndef RCControl_ESP32_h
#define RCControl_ESP32_h

#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

class RCControl_BLE
{
public:
	RCControl_BLE(const char* Service_UUID, const char* Chara_UUID, String name);
	void Begin();

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
	BLEDevice _dev;
	BLEServer *_server;
	BLEService *_serv;
	BLECharacteristic *_char;
	String _name;
	const char* SERVICE_UUID;
	const char* CHAR_UUID;

	int _NumOfItems;
	int _frontPointer;
	int _backPointer;
	String _Queue[16];

	bool JoystickUpdated;
	bool GryoUpdated;
};

#endif