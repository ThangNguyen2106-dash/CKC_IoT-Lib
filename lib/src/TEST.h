// void CKC::sendDATA(String Token_, String ID_, String Data1)
// {
//     if (WiFi.status() != WL_CONNECTED)
//     {
//         Serial.println("[CKC] WiFi not connected");
//         return;
//     }
//     WiFiClientSecure client;
//     WiFiClientSecure *client = new WiFiClientSecure;
//     client->setInsecure();
//     HTTPClient https;
//     https.begin(*client, "https://demo.api.kthd.vn/v1/cool-room");
//     https.addHeader("Content-Type", "application/json");
//     String json;
//     String dt_post = getDateTime();
//     json.reserve(200);
//     json = "{";
//     json += "\"mac_address\":\"" + Token_ + "\",";
//     json += "\"device_id\":\"" + ID_ + "\",";
//     json += "\"field_1\":" + Data1 + ",";
//     json += "\"field_2\":" + Data1 + ",";
//     json += "\"field_3\":" + Data1 + ",";
//     json += "\"field_4\":" + Data1 + ",";
//     json += "\"field_5\":" + Data1 + ",";
//     json += "\"field_6\":" + Data1 + ",";
//     json += "\"field_7\":" + Data1 + ",";
//     json += "\"field_8\":" + Data1 + ",";
//     json += "\"field_9\":" + Data1 + ",";
//     json += "\"field_10\":" + Data1 + ",";
//     json += "\"field_11\":" + Data1 + ",";
//     json += "\"post_at\":\"" + dt_post + "\"";
//     json += "}";
//     Serial.println("[CKC] Sending DATA...");
//     Serial.println(json);
//     int httpsCode = https.POST(json);
//     Serial.println("[CKC] HTTP CODE = " + String(httpsCode));
//     https.end();
//     client->stop();
// }