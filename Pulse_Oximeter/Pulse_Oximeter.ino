//#include <Arduino.h>
//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//const uint16_t port = 1025;
//const char *host = "192.168.1.199";
//WiFiClient client;
//void setup()
//{
//    Serial.begin(115200);
//    WiFi.mode(WIFI_STA);
//    WiFi.begin("Nasour", "Hn4281992@#"); // change it to your ussid and password
//    
//    while (WiFi.status() != WL_CONNECTED)
//    {
//      Serial.println("Cant connect");
//      delay(500);
//    }
//    Serial.println(WiFi.localIP());
//}
//
//void loop()
//{
//    while (!client.connect(host, port))
//    {
//       Serial.println("Could not connect to the client");
//       delay(1000);
//    }
//
// 
//     while (client.connected())
//    {
//          if(Serial.available()){
//            client.write(Serial.read()); 
//         }
////        char c = client.read();
////        client.println(c);  
//      }
//    
//    client.stop();
//    delay(5000);
//}
