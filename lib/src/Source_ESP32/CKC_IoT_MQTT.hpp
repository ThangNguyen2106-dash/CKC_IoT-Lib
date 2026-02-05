#ifndef CKC_IoT_MQTT_HPP
#define CKC_IoT_MQTT_HPP
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
class CKC_MQTT
{
private:
    String ssidSTA;
    String passSTA;
    String mqttServer;
    uint16_t mqttPort;
    String mqttid;
    String mqttUser;
    String mqttPassword;
    String Data_receive;

public:
    void begin(String WiFiID,
               String PassWiFi,
               String MQTT_server,
               uint16_t MQTT_Port,
               String MQTT_ID,
               String MQTT_user,
               String MQTT_pass);
    void run();
    void sendData(String Topic_s, String Data);
    void receiveData(String Topic_r);
};
WiFiClientSecure server;
PubSubClient mqttClient(server);
CKC_MQTT CKC_IoT_MQTT;
void CKC_MQTT::begin(String WiFiID,
                     String PassWiFi,
                     String MQTT_server,
                     uint16_t MQTT_Port,
                     String MQTT_ID,
                     String MQTT_user,
                     String MQTT_pass) // Hàm cấu hình Wifi và set up MQTT Server
{
    ssidSTA = WiFiID;
    passSTA = PassWiFi;
    mqttServer = MQTT_server;
    mqttPort = MQTT_Port;
    mqttid = MQTT_ID;
    mqttUser = MQTT_user;
    mqttPassword = MQTT_pass;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
    Serial.print("[CKC] WiFi connecting");
    unsigned long t = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t < 10000)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("\n[CKC] WiFi FAILED");
        return;
    }
    Serial.println("\n[CKC] WiFi OK");
    Serial.print("[CKC] IP: ");
    Serial.println(WiFi.localIP());
    server.setInsecure();

    mqttClient.setServer(mqttServer.c_str(), mqttPort);

    Serial.print("[CKC] MQTT connecting...");
    if (mqttClient.connect(mqttid.c_str(), mqttUser.c_str(), mqttPassword.c_str()))
    {
        Serial.println("OK");
    }
    else
    {
        Serial.print("FAILED, rc=");
        Serial.println(mqttClient.state());
    }
}

void CKC_MQTT::run() // hàm hoạt động của MQTT
{
    if (mqttClient.connected())
        mqttClient.loop();
}
void CKC_MQTT::sendData(String Topic_s, String Data) // gửi dữ liệu kèm theo TOPIC đến MQTT Server, đồng thời kiểm tra kết nỗi trước khi gửi
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[CKC] WiFi lost");
        return;
    }
    if (!mqttClient.connected())
    {
        Serial.print("[CKC] MQTT reconnect...");
        if (!mqttClient.connect(mqttid.c_str(), mqttUser.c_str(), mqttPassword.c_str()))
        {
            Serial.println("FAILED");
            return;
        }
        Serial.println("OK");
    }
    mqttClient.publish(Topic_s.c_str(), Data.c_str());
    Serial.println("[CKC] Sent: " + Topic_s + " -> " + Data);
}
#endif
