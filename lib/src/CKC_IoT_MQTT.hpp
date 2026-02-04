#ifndef _CKC_IOT_MQTT_H_
#define _CKC_IOT_MQTT_H_
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
#endif
