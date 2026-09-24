#ifndef RCControl_ESP32_h
#define RCControl_ESP32_h

#include "Arduino.h"
#include "WebServer.h"
#include "WiFi.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

class RCControl 
{
public:
	RCControl();
	virtual void Begin();

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
protected:
	bool JoystickUpdated;
	bool GryoUpdated;
private:
	int _NumOfItems;
	int _frontPointer;
	int _backPointer;
	String _Queue[16];
};

class RCControl_BLE : public RCControl
{
public:
	RCControl_BLE(const char* Service_UUID, const char* Chara_UUID, String name);
	void Begin() override;
private:
	BLEDevice _dev;
	BLEServer *_server;
	BLEService *_serv;
	BLECharacteristic *_char;
	String _name;
	const char* SERVICE_UUID;
	const char* CHAR_UUID;
};


class RCControl_WiFi : public RCControl
{
public:
	RCControl_WiFi();
	void Begin() override;
};

#endif