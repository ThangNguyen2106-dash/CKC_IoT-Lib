#include <CKC_IoT.hpp>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "updatedserver.h"
#include <Arduino.h>
IPAddress gateway(192, 168, 1, 1);
IPAddress local_ip(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ipa;
WebServer server(80);
WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
unsigned long epochTime;

CKC CKC_IoT;
String CKC::getDateTime()
{
    timeClient.update();
    time_t epoch = timeClient.getEpochTime() + 7 * 3600;
    struct tm *ptm = gmtime((time_t *)&epoch);
    if (!ptm)
    {
        return "1970-01-01T00:00:00Z";
    }
    char buffer[30];
    sprintf(buffer,
            "%04d-%02d-%02dT%02d:%02d:%02dZ",
            ptm->tm_year + 1900,
            ptm->tm_mon + 1,
            ptm->tm_mday,
            ptm->tm_hour,
            ptm->tm_min,
            ptm->tm_sec);
    return String(buffer);
}

unsigned long CKC::getTime()
{
    if (!timeClient.update())
        return 0;
    return timeClient.getEpochTime();
}

void CKC::syncTime()
{
    if (WiFi.status() != WL_CONNECTED)
        return;
    timeClient.begin();
    for (int i = 0; i < 5; i++)
    {
        if (timeClient.update())
        {
            Serial.println("[CKC] NTP synced");
            return;
        }
        delay(1000);
    }
    Serial.println("[CKC] NTP sync failed");
}

void CKC::begin(String sta_ssid, String sta_pass)
{
    ssidSTA = sta_ssid;
    passSTA = sta_pass;
    WiFi.begin(ssidSTA, passSTA);
    Serial.print("[CKC] Wifi connecting");
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime <= 10000)
    {
        delay(1000);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        Serial.println("[CKC] Connected! ESP32 IP: " + WiFi.localIP().toString());
        CKC_IoT.syncTime();
    }
    else
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP-AP MODE", "123456789", 1);
        IPAddress IP = WiFi.softAPIP();
        Serial.println("\n[CKC] CAN'T CONNECT TO WiFI");
        Serial.println("[CKC] SWITCHING TO AP MODE");
        Serial.println("[CKC] ESP32 AP IP address: " + IP.toString());
        Serial.println("[CKC] SSID: ESP-AP MODE");
        Serial.println("[CKC] Pass: 123456789");
    }
}

void CKC::end()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("[CKC] WiFi stopped");
}

void CKC::sendDATA(String Token_, String ID_, String Data1)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[CKC] WiFi not connected");
        return;
    }
    static WiFiClientSecure client;
    client.setInsecure();
    static HTTPClient https;
    https.begin(client, "https://demo.api.kthd.vn/v1/cool-machine");
    https.addHeader("Content-Type", "application/json");
    String json;
    String dt_post = getDateTime();
    json.reserve(200);
    json = "{";
    json += "\"mac_address\":\"" + Token_ + "\",";
    json += "\"device_id\":\"" + ID_ + "\",";
    json += "\"field_1\":" + Data1 + ",";
    json += "\"field_2\":" + Data1 + ",";
    json += "\"field_3\":" + Data1 + ",";
    json += "\"field_4\":" + Data1 + ",";
    json += "\"field_5\":" + Data1 + ",";
    json += "\"field_6\":" + Data1 + ",";
    json += "\"field_7\":" + Data1 + ",";
    json += "\"field_8\":" + Data1 + ",";
    json += "\"field_9\":" + Data1 + ",";
    json += "\"field_10\":" + Data1 + ",";
    json += "\"field_11\":" + Data1 + ",";
    json += "\"post_at\":\"" + dt_post + "\"";
    json += "}";
    Serial.println("[CKC] Sending DATA...:");
    Serial.print(json);
    int httpsCode = https.POST(json);
    Serial.println("[CKC] HTTP CODE = " + String(httpsCode));
    https.end();
    client.stop();
}

void CKC::readDATA(String &Data1, String &Data2, String &Data3, String &Data4, String &Data5)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        reconnectWifi(ssidSTA, passSTA);
        return;
    }

    HTTPClient http;
    http.begin("http://your-server/read.php"); // URL của bạn
    int httpCode = http.GET();

    if (httpCode > 0)
    {
        String payload = http.getString();

        int p1 = payload.indexOf('|');
        int p2 = payload.indexOf('|', p1 + 1);
        int p3 = payload.indexOf('|', p2 + 1);
        int p4 = payload.indexOf('|', p3 + 1);
        Data1 = payload.substring(0, p1);
        Data2 = payload.substring(p1 + 1, p2);
        Data3 = payload.substring(p2 + 1, p3);
        Data4 = payload.substring(p3 + 1, p4);
        Data5 = payload.substring(p4 + 1);
    }
    http.end();
}

void CKC::reconnectWifi(String sta_ssid, String sta_pass)
{
    ssidSTA = sta_ssid;
    passSTA = sta_pass;
    static unsigned long lastTry = 0;
    if (millis() - lastTry < 5000)
        return;
    lastTry = millis();
    if (WiFi.status() == WL_CONNECTED)
        return;
    if (WiFi.getMode() == WIFI_AP)
    {
        Serial.println("[CKC] AP mode: checking WiFi...");
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
        unsigned long t = millis();
        while (WiFi.status() != WL_CONNECTED &&
               millis() - t < 5000)
        {
            delay(300);
            Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\n[CKC] WiFi FOUND → STA CONNECTED");
            Serial.println(WiFi.localIP());
        }
        else
        {
            Serial.println("\n[CKC] WiFi NOT FOUND → back to AP");
            WiFi.disconnect(true);
            WiFi.mode(WIFI_AP);
            WiFi.softAP("ESP-AP MODE", "123456789", 1);
        }
    }
    else
    {
        Serial.println("[CKC] STA lost → reconnecting");
        WiFi.disconnect();
        WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
    }
}