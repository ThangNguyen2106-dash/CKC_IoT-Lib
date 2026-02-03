#include <CKC_IoT_MQTT.hpp>
WiFiClientSecure server;
PubSubClient mqttClient(server);
CKC_MQTT CKC_IoT_MQTT;
void CKC_MQTT::begin(String WiFiID, String PassWiFi, String MQTT_server, uint16_t MQTT_Port, String MQTT_ID, String MQTT_user, String MQTT_pass)
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
void CKC_MQTT::run()
{
    if (mqttClient.connected())
        mqttClient.loop();
}

void CKC_MQTT::sendData(String Topic_s, String Data)
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
void CKC_MQTT::receiveData(String Topic_r)
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
    mqttClient.subscribe(Topic_r.c_str());
}
