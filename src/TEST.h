#include <khaibao.h>
#include <CKC_IoT.h>

const char *Ten_WiFi = "MakerSpaceLab_2.4Ghz";
const char *Pass_WiFi = "Maker2025";

const char *MQTT_Server = "76257abadb724de09f59f274b3c3e098.s1.eu.hivemq.cloud";
const int16_t MQTT_PORT = 8883;

const char *MQTT_ID = "76257abadb724de09f59f274b3c3e098";
const char *MQTT_USERNAME = "hivemq.webclient.1770102461698";
const char *MQTT_PASS = "9pf7TvU#&G3rKo<>g8DX";

int timeupdate = 10000;
void setup()
{
  pinMode(26, OUTPUT);
  Serial.begin(115200);
  CKC_IoT_MQTT.begin(Ten_WiFi, Pass_WiFi, MQTT_Server, MQTT_PORT, MQTT_ID, MQTT_USERNAME, MQTT_PASS);
}
void loop()
{
  CKC_IoT_MQTT.run();
  static unsigned long lastSend = 0;
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(26, HIGH);

    if (millis() - lastSend >= timeupdate)
    {
      lastSend = millis();
      float temp = random(250, 350) / 10.0;
      float humidity = random(70, 80);
      CKC_IoT_MQTT.sendData("NHIET", String(temp));
      CKC_IoT_MQTT.sendData("DO AM", String(humidity));
    }
  }
  else
  {
    digitalWrite(26, LOW);
  }
}