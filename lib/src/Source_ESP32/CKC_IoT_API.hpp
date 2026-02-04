#ifndef CKC_IoT_API_HPP
#define CKC_IoT_API_HPP
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
WiFiClientSecure client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
unsigned long epochTime;

class CKC_API
{
public:
    CKC_API() {};
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
CKC_API CKC_IoT_API;
String CKC_API::getDateTime()
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

unsigned long CKC_API::getTime()
{
    if (!timeClient.update())
        return 0;
    return timeClient.getEpochTime();
}

void CKC_API::syncTime()
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

void CKC_API::begin(String sta_ssid, String sta_pass)
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
        CKC_IoT_API.syncTime();
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

void CKC_API::reconnectWifi(String sta_ssid, String sta_pass)
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

void CKC_API::end()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("[CKC] WiFi stopped");
}

void CKC_API::sendDATA(String Token_, String ID_, String Data1)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[CKC] WiFi not connected");
        return;
    }
    static WiFiClientSecure client;
    client.setInsecure(); // hàm cài đặt bỏ qua bước xác thực
    static HTTPClient https;
    https.begin(client, "https://demo.api.kthd.vn/v1/cool-machine");
    https.addHeader("Content-Type", "application/json");
    String data_send;
    String dt_post = getDateTime();
    data_send.reserve(200); // hàm xin chổ trống ram cho chuỗi kí tự là 200 kí tự
    data_send = "{";
    data_send += "\"mac_address\":\"" + Token_ + "\",";
    data_send += "\"device_id\":\"" + ID_ + "\",";
    data_send += "\"field_1\":" + Data1 + ",";
    data_send += "\"field_2\":" + Data1 + ",";
    data_send += "\"field_3\":" + Data1 + ",";
    data_send += "\"field_4\":" + Data1 + ",";
    data_send += "\"field_5\":" + Data1 + ",";
    data_send += "\"field_6\":" + Data1 + ",";
    data_send += "\"field_7\":" + Data1 + ",";
    data_send += "\"field_8\":" + Data1 + ",";
    data_send += "\"field_9\":" + Data1 + ",";
    data_send += "\"field_10\":" + Data1 + ",";
    data_send += "\"field_11\":" + Data1 + ",";
    data_send += "\"post_at\":\"" + dt_post + "\"";
    data_send += "}";
    Serial.print("[CKC] Sending DATA...:");
    Serial.print(data_send);
    int httpsCode = https.POST(data_send); // hàm kiểm tra lỗi và post data lên server
    Serial.println("\n[CKC] HTTP CODE = " + String(httpsCode));
    https.end();
    client.stop();
}
//-------------------------Coming Soon---------------------------//
void CKC_API::readDATA()
{
}
#endif