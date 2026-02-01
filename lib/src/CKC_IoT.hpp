#ifndef CKC_H
#define CKC_H
#include <WiFi.h>
#define CKC_MAX_VPIN 10
struct CKC_VPin
{
    String pin;
    String value;
};
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
    void readDATA(String &Data1, String &Data2, String &Data3, String &Data4, String &Data5);
    void reconnectWifi(String sta_ssid, String sta_pass);

private:
    String ssidSTA, passSTA;
    String Data1, Data2, Data3, Data4, Data5;
};
#endif