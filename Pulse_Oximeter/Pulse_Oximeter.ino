// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
// const uint16_t port = 1025;
// const char *host = "192.168.1.199";
// WiFiClient client;
// void setup()
// {
//    Serial.begin(115200);
//    Serial.println("HI");
//    WiFi.mode(WIFI_STA);
//    WiFi.begin("Nasour", "Hn4281992@#"); // change it to your ussid and password
   
//    while (WiFi.status() != WL_CONNECTED)
//    {
//      Serial.println("Cant connect");
//      delay(500);
//    }
//    Serial.println(WiFi.localIP());
// }

// void loop()
// {
//    char c;
//    while (!client.connect(host, port))
//    {
//       Serial.println("Could not connect to the client");
//       delay(1000);
//    }
//
// Stop until server gives the go-ahead when user enters name
//    if(client.available()){
//        c = client.read();
//    }
//
//     while (client.connected())
//    {
//          if(Serial.available()){
//            client.write(Serial.read()); 
//         }
// //        c = client.read();
// //        client.println(c);  
//      }
   
//    client.stop();
//    delay(5000);
// }
// //
// //void setup(){}
// //void loop(){}
