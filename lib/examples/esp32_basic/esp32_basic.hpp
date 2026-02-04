// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiMulti.h>
// #include <HTTPClient.h>
// #include <WiFiClientSecure.h>
// #include <WiFiClient.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #include <EEPROM.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>
// #include <Update.h>
// #include "updateserver.h"

// #include <ModbusRTUMaster.h>
// #include <SoftwareSerial.h>

// const int addr_offset = 40;
// const int addr_ssid_offset = 70;
// const int addr_pass_offset = 100;
// #define SSID_LEN 30
// #define PASS_LEN 20
// // Put your SSID & Password for ESP Access Point
// const char *host = "esp32";
// // const char *ssidex = "KTHD_Meter";     // Enter SSID here
// // const char *passwordex = "123456";  //Enter Password here
// char *ssidn, *passn;
// String wifiid = "", wifipass = "";
// // Put IP Address details
// IPAddress gateway(192, 168, 1, 1);
// IPAddress local_ip(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress ipa;
// WebServer server(80); /*Set up webserver for OTA firmware update*/
// WiFiServer configWebServer(80);
// WiFiClient client;
// uint16_t time_change_mode = 60;
// uint16_t count_time = 0;
// uint16_t count_data = 0;
// uint16_t count_post = 0;
// uint16_t count_reset_server = 0;
// unsigned long lastConnectionTime = 0, timer_set_bt = 0; // last time you connected to the server, in milliseconds
// unsigned long timer_run = 0, factor_timer = 1;
// const unsigned long postingInterval = 1000;             // delay between updates, in milliseconds
// const unsigned long cycle_post = 60, resetServer = 150; // delay between updates, in seconds
// String MAC = "";
// #define status_wifi 2
// // ModbusRTU mb;
// uint8_t slave_meter = 1;
// #define total_meter 1
// SoftwareSerial serialESP;
// bool check_read = false;
// const uint8_t rxPin = 16;
// const uint8_t txPin = 17;
// const uint8_t dePin = 13;
// ModbusRTUMaster mb(Serial2); // serial port, driver enable pin for rs-485 (optional)

// float v1 = 0, v2 = 0, v3 = 0;
// float c1 = 0, c2 = 0, c3 = 0;
// float pf1 = 0, pf2 = 0, pf3 = 0;
// float freq = 50;
// float power1 = 0, power2 = 0, power3 = 0;
// float total_act_power = 0, total_react_power = 0;
// float total_energy = 0;
// float t[6] = {0, 0, 0, 0, 0, 0};
// // int NTC[6] = {25,26,27,14,12,13};
// int NTC[6] = {33, 32, 35, 34, 39, 36};
// // int ADC_offset = 2677 - 2590;//KTHD-01
// // int ADC_offset = 2550 - 2590; //KTHD-03
// int ADC_offset = 0;
// const int ADC_ref = 1907;

// WiFiMulti WiFiMulti;
// const uint32_t connectTimeoutMs = 5000;
// // Define NTP Client to get time
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org");

// // Variable to save current epoch time
// unsigned long epochTime;

// // Function that gets current epoch time
// unsigned long getTime()
// {
//   timeClient.update();
//   unsigned long now = timeClient.getEpochTime();
//   return now;
// }
// // Function that gets current epoch time
// String getDateTime()
// {
//   timeClient.update();
//   time_t epoch = timeClient.getEpochTime();
//   struct tm *ptm = gmtime((time_t *)&epoch);
//   String month_temp = "";
//   if ((ptm->tm_mon + 1) < 10)
//     month_temp = "0" + String(ptm->tm_mon + 1);
//   else
//     month_temp = String(ptm->tm_mon + 1);
//   String date_temp = "";
//   if ((ptm->tm_mday) < 10)
//     date_temp = "0" + String(ptm->tm_mday);
//   else
//     date_temp = String(ptm->tm_mday);
//   String datetime = String(ptm->tm_year + 1900) + "-" + month_temp + "-" + date_temp + "T" + timeClient.getFormattedTime() + "Z";
//   // Serial.print("Datetime; ");
//   // Serial.println(datetime);
//   return datetime;
// }

// // Initialize WiFi
// bool Set_wifi_mode = false;
// bool initWiFi()
// {
//   // if(wifiid=="" || wifipass==""){
//   //   Serial.println("Undefined SSID or IP address.");
//   //   return false;
//   // }
//   Serial.println("Connecting to WiFi...");
//   Serial.println(wifiid);
//   Serial.println(wifipass);
//   int timer = 0;
//   while (WiFiMulti.run() != WL_CONNECTED)
//   {
//     delay(1000);
//     Serial.print(".");
//     timer++;
//     if (timer >= 10)
//       break;
//   }
//   //  delay(20000);
//   if (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.println("Failed to connect.");
//     return false;
//   }

//   Serial.println(WiFi.localIP());
//   return true;
// }
// String inputString = "";     // a String to hold incoming data
// bool stringComplete = false; // whether the string is complete
// bool data_en = false;
// bool modbus_err = false;

// void setup()
// {
//   pinMode(status_wifi, OUTPUT);
//   digitalWrite(status_wifi, HIGH);
//   Serial.begin(115200);
//   Serial2.begin(9600, SERIAL_8N1, 16, 17, false);
//   serialESP.begin(9600, SWSERIAL_8N1, 5, 4, false);
//   mb.begin(9600, SERIAL_8N1);
//   // reserve 200 bytes for the inputString:
//   inputString.reserve(200);
//   EEPROM.begin(512);
//   delay(10);

//   wifiid = readStringFromEEPROM(addr_ssid_offset);
//   wifipass = readStringFromEEPROM(addr_pass_offset);
//   delay(1000);
//   const char *cid = wifiid.c_str();
//   const char *cpass = wifipass.c_str();

//   WiFi.mode(WIFI_STA);
//   WiFiMulti.addAP(cid, cpass);
//   WiFiMulti.addAP("other_wifi", "other_pass");

//   if (initWiFi())
//   {
//     Serial.print("Wifi is connected ");
//     MAC = WiFi.macAddress();
//     Serial.println(MAC);
//     Serial.println();
//     digitalWrite(status_wifi, LOW);

//     run_init_OTA();
//     //    writeStringtoEEPROM(addr_ssid_offset,wifiid);
//     //    writeStringtoEEPROM(addr_pass_offset,wifipass);
//   }
//   else
//   {
//     Set_wifi_mode = true;
//     WiFi.mode(WIFI_AP_STA); // starts the default AP (factory default or setup as persistent)
//     MAC = WiFi.macAddress();
//     Serial.println(MAC);
//     Serial.println();
//     String ssid_t = "KTHD-" + MAC;
//     const char *ssidex = ssid_t.c_str(); // Enter SSID here
//     const char *passwordex = "123456";   // Enter Password here
//     WiFi.softAP(ssidex, passwordex);
//     WiFi.softAPConfig(local_ip, gateway, subnet);
//     delay(100);
//     Serial.println(" ");
//     Serial.print("Connect your computer to the WiFi network: ");
//     Serial.print(ssidex);
//     Serial.print("  password: ");
//     Serial.print(passwordex);
//     Serial.print(" and enter http://");
//     Serial.print(local_ip);
//     Serial.println(" in a Web browser");

//     configWebServer.begin();
//     WiFi.disconnect();
//     WiFi.softAPConfig(local_ip, gateway, subnet);
//     if (WiFi.softAPIP() == local_ip)
//     {
//       Serial.println(WiFi.softAP(ssidex) ? "soft-AP setup" : "Failed to connect");
//       Serial.println(WiFi.softAPIP());
//     }
//   }
//   // timeClient.setTimeOffset(+25200);
//   timeClient.setTimeOffset(0);
//   Get_temperature();
//   ADC_offset = t[5] - ADC_ref;
//   Get_data_meter(slave_meter);
//   delay(1000);
// }

// void loop()
// {
//   if (Set_wifi_mode)
//   {
//     client = configWebServer.available();
//     char line[64];
//     String lineMessage = "";
//     int l = client.readBytesUntil('\n', line, sizeof(line));
//     line[l] = 0;

//     client.find((char *)"\r\n\r\n");
//     if (strncmp_P(line, PSTR("POST"), strlen("POST")) == 0)
//     {
//       l = client.readBytes(line, sizeof(line));
//       line[l] = 0;
//       for (int i = 0; i < sizeof(line); i++)
//       {
//         if (line[i] == '+')
//         {
//           line[i] = ' ';
//         }
//       }
//       Serial.println(line);
//       // parse the parameters sent by the html form
//       const char *delims = "=&";
//       strtok(line, delims);
//       const char *ssid = strtok(NULL, delims);
//       strtok(NULL, delims);
//       const char *pass = strtok(NULL, delims);
//       strtok(NULL, delims);

//       // send a response before attemting to connect to the WiFi network
//       // because it will reset the SoftAP and disconnect the client station
//       client.println(F("HTTP/1.1 200 OK"));
//       client.println(F("Connection: close"));
//       client.println(F("Refresh: 10")); // send a request after 10 seconds
//       client.println();
//       client.println(F("<html><body><h3>Configuration AP</h3><br>connecting...</body></html>"));

//       Serial.println();
//       Serial.print("Attempting to connect to WPA SSID: ");
//       // Serial.println(ssid);
//       // Serial.println(pass);
//       ssidn = strdup(ssid);
//       passn = strdup(pass);
//       // wifiid = String(ssidn);
//       // wifipass = String(passn);
//       wifiid = urlDecode(ssidn);
//       wifipass = urlDecode(passn); // urlDecode
//       Serial.println(wifiid);
//       Serial.println(wifipass);
//       writeStringtoEEPROM(addr_ssid_offset, wifiid);
//       writeStringtoEEPROM(addr_pass_offset, wifipass);

//       delay(5);
//       EEPROM.commit();
//       //        WiFi.waitForConnectResult();

//       // configuration continues with the next request
//       client.println(F("HTTP/1.1 200 OK"));
//       client.println(F("Connection: close"));
//       client.println();
//       client.println(F("<html><body><h3>Configuration AP</h3><br>"));

//       client.println(F("Wrote ssid and password to memory. Ending AP."));
//       client.println("ESP MAC address: ");
//       client.print(MAC);
//       // client.println(F("<form action='/' method='POST'>WiFi connection failed. Enter valid parameters, please.<br><br>"));

//       lineMessage = "SSID:<br><input type='text' value='" + String(wifiid) + "' name='i'><br>";
//       client.println(lineMessage);
//       lineMessage = "Password:<br><input type='password' value='" + String(wifipass) + "' name='p'><br><br>";
//       client.println(lineMessage);

//       client.println(F("<input type='submit' value='Submit'></form>"));

//       client.println(F("</body></html>"));
//       client.stop();
//       ESP.restart();
//     }
//     else
//     {

//       client.println(F("HTTP/1.1 200 OK"));
//       client.println(F("Connection: close"));
//       client.println();
//       client.println(F("<html><body><h3>Configuration AP</h3><br>"));

//       int status = WiFi.status();
//       if (status == WL_CONNECTED)
//       {
//         client.println(F("Connection successful. Ending AP."));
//       }
//       else
//       {
//         client.print("ESP MAC address: ");
//         client.println(MAC);
//         client.println(F("<form action='/' method='POST'>WiFi connection is saved to memory.<br><br>"));

//         lineMessage = "SSID:<br><input type='text' value='" + String(wifiid) + "' name='i'><br>";
//         client.println(lineMessage);
//         lineMessage = "Password:<br><input type='password' value='" + String(wifipass) + "' name='p'><br><br>";
//         client.println(lineMessage);

//         client.println(F("<input type='submit' value='Submit'></form>"));
//       }
//       client.println(F("</body></html>"));
//       client.stop();
//     }
//     if (count_reset_server >= resetServer)
//     {
//       ESP.restart();
//     }
//   }

//   if (WiFi.status() == WL_CONNECTED)
//   {
//     server.handleClient();
//   }

//   if (millis() < lastConnectionTime)
//     lastConnectionTime = millis();
//   if ((millis() - lastConnectionTime >= postingInterval))
//   {
//     lastConnectionTime = millis();
//     count_time++;

//     if (Set_wifi_mode)
//       count_reset_server++;
//     else
//       count_reset_server = 0;
//     if (modbus_err)
//     {
//       digitalWrite(status_wifi, !digitalRead(status_wifi));
//     }
//     if (count_time >= cycle_post)
//     {
//       count_time = 0;
//       slave_meter = 1;
//       Get_data_meter(slave_meter);
//       Get_temperature();
//       // if (WiFi.status() == WL_CONNECTED) {
//       if (WiFiMulti.run() == WL_CONNECTED)
//       {
//         digitalWrite(status_wifi, LOW);
//         String dt_post = getDateTime();
//         // std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
//         WiFiClientSecure *client = new WiFiClientSecure;
//         // client->setFingerprint(fingerprint);
//         //  Or, if you happy to ignore the SSL certificate, then use the following line instead:
//         client->setInsecure();
//         HTTPClient https;
//         String IP_Local = WiFi.localIP().toString();
//         Serial.print("[HTTPS] begin...\n");
//         //// post data to 4Ps:

//         if (https.begin(*client, "https://demo.api.kthd.vn/v1/cool-room"))
//         { // HTTPS

//           https.addHeader("Content-Type", "application/json");
//           // https.addHeader("x-api-key", "177FBAE90B7A40428D73B4EF677879FF");
//           String data_post = "{\"mac_address\": \"" + MAC + "\",";
//           data_post += "\"device_id\": \"KTHD_Meter - " + String(slave_meter) + "\",";
//           data_post += "\"field_1\": " + String(c1) + ",";
//           data_post += "\"field_2\": " + String(c2) + ",";
//           data_post += "\"field_3\": " + String(c3) + ",";
//           data_post += "\"field_4\": " + String(t[0]) + ",";
//           data_post += "\"field_5\": " + String(t[1]) + ",";
//           data_post += "\"field_6\": " + String(t[2]) + ",";
//           data_post += "\"field_7\": " + String(t[3]) + ",";
//           data_post += "\"field_8\": " + String(t[4]) + ",";
//           data_post += "\"field_9\": " + String(t[5]) + ",";
//           data_post += "\"field_10\": " + String(total_energy) + ",";
//           data_post += "\"field_11\": " + String(ADC_offset) + ",";
//           data_post += "\"post_at\": \"" + dt_post + "\"";
//           data_post += "}";
//           Serial.println(data_post);
//           int httpResponseCode = https.POST(data_post);
//           Serial.println(httpResponseCode);
//           https.end();
//           if (httpResponseCode < 0)
//           {
//             ESP.restart();
//           }
//         }
//         else
//         {
//           Serial.printf("[HTTPS] Unable to connect\n");
//         }
//       }
//       else
//       {
//         if (!modbus_err)
//         {
//           Serial.printf("Wifi Unable to connect\n");
//           digitalWrite(status_wifi, HIGH);
//           // reconnect wifi:
//           // initWiFi();
//         }
//       }
//     }
//   }
// }
// void Get_data_meter(uint8_t slaveid)
// {
//   uint16_t size_data = 10;
//   uint16_t res[size_data];
//   uint32_t data_t[size_data];
//   uint8_t result = 0;

//   // Get volt&current phase1&2&3:
//   result = mb.readInputRegisters(slave_meter, 0, res, size_data);
//   if (result == MODBUS_RTU_MASTER_SUCCESS)
//   {
//     modbus_err = false;
//     for (int i = 0; i < size_data; i++)
//     {
//       data_t[i] = res[i];
//       // Serial.println(data_t[i]);
//     }
//     v1 = data_t[0] / 10.0f;
//     // v1 = Convert_2_float(res[0],res[1]);
//     v2 = 0; // Convert_2_float(res[2],res[3]);
//     v3 = 0; // Convert_2_float(res[4],res[5]);
//     c1 = (data_t[1] + (data_t[2] << 16)) / 1000.0f;
//     // c1 = Convert_2_float(res[8],res[9]);
//     c2 = 0; // Convert_2_float(res[10],res[11]);
//     c3 = 0; // Convert_2_float(res[12],res[13]);

//     total_energy = (data_t[5] + (data_t[6] << 16)) / 1000.0f;
//   }
//   else
//   {
//     modbus_err = true;
//     // v1 = 0; v2 = 0; v3 = 0; c1 = 0; c2 = 0; c3 = 0;
//   }
//   Serial.println(v1);
//   Serial.println(c1);
//   Serial.println(total_energy);
//   delay(500);
// }
// float Convert_2_float(uint16_t u1, uint16_t u2)
// {
//   byte hexArray[4];
//   float value = 0.0;
//   hexArray[3] = (u1 >> 8) & 0xFF;
//   hexArray[2] = u1 & 0xFF;
//   hexArray[1] = (u2 >> 8) & 0xFF;
//   hexArray[0] = u2 & 0xFF;

//   memcpy(&value, hexArray, 4);
//   //  Serial.println(value);
//   return value;
// }
// String readStringFromEEPROM(int addrOffset)
// {
//   int len = 0;
//   if (addrOffset == addr_ssid_offset)
//     len = SSID_LEN;
//   else if (addrOffset == addr_pass_offset)
//     len = SSID_LEN;
//   else
//     return "";

//   String str = "";
//   char c = (char)EEPROM.read(addrOffset);
//   if (c != '#')
//     return str;

//   for (int i = 1; i <= len; i++)
//   {
//     c = (char)EEPROM.read(addrOffset + i);
//     if (c != '$')
//       str += c;
//     else
//       break;
//   }
//   return str;
// }
// void writeStringtoEEPROM(int addrOffset, String data_wifi)
// {
//   uint8_t len = data_wifi.length();
//   EEPROM.write(addrOffset, '#');
//   for (int i = 1; i <= len; i++)
//   {
//     EEPROM.write(addrOffset + i, data_wifi[i - 1]);
//   }
//   EEPROM.write(addrOffset + len + 1, '$');
//   if (EEPROM.commit())
//   {
//     Serial.println("EEPROM successfully committed");
//   }
//   else
//   {
//     Serial.println("ERROR! EEPROM commit failed");
//   }
// }
// String urlDecode(String input)
// {
//   String decoded = "";
//   char temp[] = "0x00";
//   unsigned int len = input.length();
//   unsigned int i = 0;
//   while (i < len)
//   {
//     char c = input.charAt(i);
//     if (c == '+')
//     {
//       decoded += ' ';
//     }
//     else if (c == '%')
//     {
//       if (i + 2 < len)
//       {
//         temp[2] = input.charAt(i + 1);
//         temp[3] = input.charAt(i + 2);
//         decoded += (char)strtol(temp, NULL, 16);
//         i += 2;
//       }
//     }
//     else
//     {
//       decoded += c;
//     }
//     i++;
//   }
//   return decoded;
// }
// void Get_temperature(void)
// {
//   int temp[6] = {0, 0, 0, 0, 0, 0};
//   int num = 10;
//   for (int i = 0; i < num; i++)
//   {
//     for (int j = 0; j < 6; j++)
//     {
//       temp[j] += analogRead(NTC[j]);
//       delay(1);
//     }
//   }
//   for (int i = 0; i < 6; i++)
//   {
//     t[i] = float(temp[i]) / num - ADC_offset;
//     Serial.println(t[i]);
//   }
// }
// void run_init_OTA()
// {
//   /*use mdns for host name resolution*/
//   if (!MDNS.begin(host))
//   { // http://esp32.local
//     Serial.println("Error setting up MDNS responder!");
//     while (1)
//     {
//       delay(1000);
//     }
//   }
//   Serial.println("mDNS responder started");
//   /*return index page which is stored in serverIndex */
//   server.on("/", HTTP_GET, []()
//             {
//     server.sendHeader("Connection", "close");
//     server.send(200, "text/html", loginIndex); });
//   server.on("/serverIndex", HTTP_GET, []()
//             {
//     server.sendHeader("Connection", "close");
//     server.send(200, "text/html", serverIndex); });
//   /*handling uploading firmware file */
//   server.on("/update", HTTP_POST, []()
//             {
//     server.sendHeader("Connection", "close");
//     server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
//     ESP.restart(); }, []()
//             {
//     HTTPUpload& upload = server.upload();
//     if (upload.status == UPLOAD_FILE_START) {
//       Serial.printf("Update: %s\n", upload.filename.c_str());
//       if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
//         Update.printError(Serial);
//       }
//     } else if (upload.status == UPLOAD_FILE_WRITE) {
//       /* flashing firmware to ESP*/
//       if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
//         Update.printError(Serial);
//       }
//     } else if (upload.status == UPLOAD_FILE_END) {
//       if (Update.end(true)) { //true to set the size to the current progress
//         Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
//       } else {
//         Update.printError(Serial);
//       }
//     } });
//   server.begin();
// }
