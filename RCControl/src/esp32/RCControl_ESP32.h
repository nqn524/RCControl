#ifndef RCControl_ESP32_h
#define RCControl_ESP32_h

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "../WebSockets/WebSocketsServer.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "website/INDEX_HTML.h"
#include "website/JOYSTICK_JS.h"

class RCControl 
{
public:
	RCControl();
	virtual void Begin();
	virtual void loop();

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
	void loop() override;
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
	WebServer httpServer;
	WebSocketsServer webSocket;

	RCControl_WiFi(const char* AP_SSID, const char* AP_PASS);
	void Begin() override;
	void loop() override;
private:
	static RCControl_WiFi* instance;

	static void webSocketEventStatic(uint8_t clientNum, WStype_t type, uint8_t* payload, size_t length);
	void webSocketEvent(uint8_t clientNum, WStype_t type, uint8_t* payload, size_t length);
	void handleRoot();
	void handleJavaScript();

	const char* _AP_SSID;
	const char* _AP_PASS;
};

#endif