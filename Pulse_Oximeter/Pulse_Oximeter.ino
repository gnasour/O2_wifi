// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
//
// #define BLUE_LED 2
// 
// const uint16_t port = 1025;
// const char *host = "192.168.0.16";
// WiFiClient client;
//   
// void setup()
// {
//    Serial.begin(115200);
//    WiFi.mode(WIFI_STA);
//    WiFi.begin("Priscilla-2G", "9566939939"); // change it to your ussid and password
//    pinMode(BLUE_LED, OUTPUT);
//    
//    while(WiFi.status() != WL_CONNECTED)
//    {
//      delay(1000);
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
//    
//    while (!client.connect(host, port))
//    {
////      delay(1000);
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
//   
//            client.write(Serial.read());
//         } 
//    } 
//    client.stop();
//    
// }
//
////void setup(){}
////void loop(){}
