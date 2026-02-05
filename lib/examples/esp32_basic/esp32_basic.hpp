#include <khaibao.h>
#include <CKC_IoT.h>
const char *WiFi_ID = "TÊN WIFI BẠN MUỐN";
const char *WiFi_Pass = "PASS WIFI";

const char *MQTT_Server = "đường dẫn Server";
const int16_t MQTT_PORT = 5555; // Số cổng Server

const char *MQTT_ID = "ID của MQTT";
const char *MQTT_USERNAME = "Tên USER CỦA MQTT";
const char *MQTT_PASS = "PASS CỦA MQTT";
void setup()
{
    Serial.begin(115200);
    CKC_IoT_MQTT.begin(WiFi_ID, WiFi_Pass, MQTT_Server, MQTT_PORT, MQTT_ID, MQTT_USERNAME, MQTT_PASS);
}
void loop()
{
    CKC_IoT_MQTT.run(); // khởi động MQTT
}