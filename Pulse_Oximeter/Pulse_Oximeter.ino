// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
//
// #define BLUE_LED 2
// 
// const uint16_t port = 1025;
// const char *host = "192.168.1.199";
// WiFiClient client;
//   
// void setup()
// {
//    Serial.begin(115200);
//    WiFi.mode(WIFI_STA);
//    WiFi.begin("Nasour", "Hn4281992@#"); // change it to your ussid and password
//    pinMode(BLUE_LED, OUTPUT);
//    
//    while(WiFi.status() != WL_CONNECTED)
//    {
//      Serial.println(WiFi.status());
//      digitalWrite(BLUE_LED, LOW);
//      delay(2500);
//      digitalWrite(BLUE_LED, HIGH);
//      delay(2500);
//    }
//      digitalWrite(BLUE_LED, HIGH);
//    
// }
//
// void loop()
// {
//    char c;
//    while (!client.connect(host, port))
//    {
//      
//       digitalWrite(BLUE_LED, LOW);
//       delay(250);
//       digitalWrite(BLUE_LED, HIGH);
//       delay(250);
//    }
//      digitalWrite(BLUE_LED, HIGH);
//      
//    
//
//     while (client.connected())
//    {
//          if(Serial.available()){
//            client.write(Serial.read()); 
//         } 
//    } 
//    client.stop();
//    
// }
//
////void setup(){}
////void loop(){}
