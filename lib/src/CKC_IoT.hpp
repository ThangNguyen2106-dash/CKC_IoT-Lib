#ifndef CKC_H
#define CKC_H
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
class CKC
{
public:
    CKC() {};
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
#endif