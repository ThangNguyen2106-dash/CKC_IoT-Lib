#ifndef CKC_CLASS_HPP
#define CKC_CLASS_HPP
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <PubSubClient.h>
/* ===== CKC_API ===== */
class CKC_API
{
public:
    CKC_API() {}
    String getDateTime();
    unsigned long getTime();
    void syncTime();
    void begin(String sta_ssid, String sta_pass);
    void end();
    void sendDATA(String Token_, String ID_, String Data1);
    void readDATA();
    void reconnectWifi(String sta_ssid, String sta_pass);

private:
    String ssidSTA, passSTA;
    String Data1, Data2, Data3, Data4, Data5;
};
/* ===== CKC_MQTT ===== */
class CKC_MQTT
{
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

private:
    String ssidSTA;
    String passSTA;
    String mqttServer;
    uint16_t mqttPort;
    String mqttid;
    String mqttUser;
    String mqttPassword;
    String Data_receive;
};
#endif