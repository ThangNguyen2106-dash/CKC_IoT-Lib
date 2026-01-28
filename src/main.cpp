#include <Arduino.h>
#include <CKC_IoT.h>
CKC CKC_IoT;
unsigned long lastSend = 0;
void setup()
{
  Serial.begin(115200);

  CKC_IoT.begin("PLG", "admin123");
}
void loop()
{
  if (millis() - lastSend > 5000)
  {
    lastSend = millis();
    float temp = random(250, 350) / 10.0;
    CKC_IoT.sendDATA(String(temp), "", "", "", "ESP32-01");
  }
}
