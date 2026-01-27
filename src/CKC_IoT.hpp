#include <CKC_IoT.h>
void CKC::begin(String sta_ssid, String sta_pass)
{
    ssidSTA = sta_ssid;
    passSTA = sta_pass;
    WiFi.begin(ssidSTA, passSTA);
    Serial.print("Wifi connecting");
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime <= 10000)
    {
        delay(1000);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        Serial.println("Connected! ESP32 IP: " + WiFi.localIP().toString());
    }
    else
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP-AP MODE", "123456789", 1);
        IPAddress IP = WiFi.softAPIP();
        Serial.println("\nCAN'T CONNECT TO WiFI");
        Serial.println("SWITCHING TO AP MODE");
        Serial.println("ESP32 AP IP address: " + IP.toString());
    }
}
void CKC::end()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("[CKC] WiFi stopped");
}
void CKC::sendDATA(String Data1, String Data2, String Data3, String Data4, String Data5)
{
}
void CKC::reconnectWifi()
{
    if (WiFi.getMode() == WIFI_AP)
        return;
    if (WiFi.getMode() == WIFI_STA)
        return;
    unsigned long reconnect_time = millis();
    if (millis() - reconnect_time < 5000)
        return;
    reconnect_time = millis();
    Serial.println("[CKC] WiFi lost, reconnecting...");
    CKC_IoT.end();
    CKC_IoT.begin("MakerSpaceLab", "Maker2025");
}