#include <khaibao.h>
#include <CKC_IoT.h>
const char *AUTH_TOKEN = "14:08:08:A6:41:60";
const char *ID = "PLG_TEST123";
void setup()
{
  pinMode(26, OUTPUT);
  Serial.begin(115200);
  CKC_IoT.begin("MakerSpaceLab_2.4Ghz", "Maker2025");
  String TOKEN = String(WiFi.macAddress());
}
void loop()
{
  static unsigned long lastSend = 0;
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(26, HIGH);

    if (millis() - lastSend >= 30000)
    {
      lastSend = millis(); 
      float temp = random(250, 350) / 10.0;
      CKC_IoT.sendDATA(AUTH_TOKEN, ID, String(temp));
    }
  }
  else
  {
    digitalWrite(26, LOW);
  }
}
