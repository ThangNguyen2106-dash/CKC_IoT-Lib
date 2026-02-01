// void CKC::sendDATA(String Token_, String ID_, String Data1)
// {
//     if (WiFi.status() != WL_CONNECTED)
//     {
//         Serial.println("[CKC] NO WiFi → DATA NOT SENT");
//         return;
//     }
//     HTTPClient http;
//     String url = "https://demo.api.kthd.vn/v1/PLG_TEST";
//     http.begin(url);
//     http.addHeader("Content-Type", "application/json");
//     String dt_post = CKC_IoT.getDateTime();
//     String json = "{";
//     json += "\"mac_address\":\"" + Token_ + "\",";
//     json += "\"device_id\":\"" + ID_ + "\",";
//     json += "\"field_8\":\"" + Data1 + "\",";
//     json += "\"post_at\":\"" + dt_post + "\"";
//     json += "}";
//     Serial.println(Token_);
//     Serial.println("[CKC] Sending DATA...");
//     Serial.println(json);
//     int httpCode = http.POST(json);
//     if (httpCode > 0)
//     {
//         Serial.println("[CKC] DATA SENT OK");
//         Serial.println("[CKC] HTTP CODE: " + String(httpCode));
//         Serial.println(http.getString());
//     }
//     else
//     {
//         Serial.println("[CKC] SEND FAILED");
//         Serial.println("[CKC] ERROR: " + String(httpCode));
//     }
//     http.end();
// }