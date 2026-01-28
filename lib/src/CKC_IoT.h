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
    void begin(String sta_ssid, String sta_pass);
    void end();
    void sendDATA(String Data1, String Data2, String Data3, String Data4, String Data5);
    void reconnectWifi(String sta_ssid, String sta_pass);
    void virtualWrite(String pin_, float value_);
    void virtualWrite(String pin_, int value_);
    void virtualRead(String pin_);
    void flushVirtual();

private:
    String ssidSTA, passSTA;
    String data1, data2, data3, data4, data5;
    CKC_VPin vpin[CKC_MAX_VPIN];
    uint8_t vpinCount = 0;
};
#endif
#include<CKC_IoT.hpp>