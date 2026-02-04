#include <khaibao.h>
#include <CKC_IoT.h>
const char *Ten_WiFi = "MakerSpaceLab_2.4Ghz";
const char *Pass_WiFi = "Maker2025";
const char *AUTH_TOKEN = "14:08:08:A6:41:60";
const char *ID = "PLG_TEST123";
int timeupdate = 10000;
void setup()
{
  pinMode(26, OUTPUT);
  Serial.begin(115200);
  CKC_IoT_API.begin(Ten_WiFi, Pass_WiFi);
  String TOKEN = String(WiFi.macAddress());
}
void loop()
{
  static unsigned long lastSend = 0;
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(26, HIGH);

    if (millis() - lastSend >= timeupdate)
    {
      lastSend = millis();
      float temp = random(250, 350) / 10.0;
      CKC_IoT_API.sendDATA(AUTH_TOKEN, ID, String(temp));
    }
  }
  else
  {
    digitalWrite(26, LOW);
  }
}