#include <RCControl.h>

RCControl_BLE RCC("12345678-1234-1234-1234-123456789abc", "abcdef01-1234-1234-1234-123456789abc", "Example");

void setup() {
  Serial.begin(9600);

  RCC.Begin();

  Serial.println("Waiting for BLE messages");
}

void loop() {
  if (RCC.connected) {
    if (RCC.JoystickUpdate()) {
      Serial.println(String(RCC.JoyStickX) + "   " + String(RCC.JoyStickY));
    }

    if (RCC.GyroUpdate()) {
      Serial.println(String(RCC.GyroA) + "   " + String(RCC.GyroB) + "   " + String(RCC.GyroG));
    }

    if (!RCC.Empty()) {
      Serial.println("Queue: " + RCC.Dequeue());
    }
  }
}