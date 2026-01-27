#include "CKC_IoT.h"

CKC::CKC() {}

void CKC::begin(String sta_ssid, String sta_pass)
{
    ssidSTA = sta_ssid;
    passSTA = sta_pass;

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);
    WiFi.begin(ssidSTA.c_str(), passSTA.c_str());

    Serial.print("Wifi connecting");

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED &&
           millis() - startAttemptTime <= 10000)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        staOK = true;
        apMode = false;

        Serial.println("\n[CKC] STA CONNECTED");
        Serial.println("IP: " + WiFi.localIP().toString());
    }
    else
    {
        staOK = false;
        apMode = true;

        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP-AP MODE", "123456789", 1);

        Serial.println("\n[CKC] CAN'T CONNECT TO WIFI");
        Serial.println("[CKC] SWITCHING TO AP MODE");
        Serial.println("AP IP: " + WiFi.softAPIP().toString());
    }
}

void CKC::loop()
{
    // chỉ reconnect khi đã từng STA OK
    if (!staOK || apMode)
        return;

    if (WiFi.status() == WL_CONNECTED)
        return;

    unsigned long now = millis();
    if (now - lastReconnectAttempt >= RECONNECT_INTERVAL)
    {
        lastReconnectAttempt = now;

        Serial.println("[CKC] WiFi lost → reconnecting...");
        WiFi.disconnect();
        WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
    }
}

void CKC::end()
{
    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_OFF);
    staOK = false;
    apMode = false;
    Serial.println("[CKC] WiFi stopped");
}

bool CKC::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void CKC::sendDATA(String Data1,
                   String Data2,
                   String Data3,
                   String Data4,
                   String Data5)
{
    if (!isConnected())
    {
        Serial.println("[CKC] No WiFi → sendDATA skipped");
        return;
    }

    HTTPClient http;

    String url = "http://your-server-ip/data"; // 🔧 đổi thành server của bạn
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"data1\":\"" + Data1 + "\",";
    json += "\"data2\":\"" + Data2 + "\",";
    json += "\"data3\":\"" + Data3 + "\",";
    json += "\"data4\":\"" + Data4 + "\",";
    json += "\"data5\":\"" + Data5 + "\"";
    json += "}";

    int httpCode = http.POST(json);

    if (httpCode > 0)
    {
        Serial.println("[CKC] DATA SENT OK");
        Serial.println(http.getString());
    }
    else
    {
        Serial.println("[CKC] SEND FAILED");
    }

    http.end();
}
