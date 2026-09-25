#include <RCControl.h>

RCControl_WiFi RCC("Example", "password");

void setup() {
  Serial.begin(115200);

  RCC.Begin();

  Serial.println("Online");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  RCC.loop();
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
