#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
const uint16_t port = 1025;
const char *host = "192.168.1.199";
WiFiClient client;
void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin("Nasour", "Hn4281992@#"); // change it to your ussid and password
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    }
    Serial.println(WiFi.localIP());
}

void loop()
{
    if (!client.connect(host, port))
    {
        return;
    }
    
     while (client.available() > 0)
    {
         client.println("Hello From ESP8266");
         delay(5000);
 
//        char c = client.read();
//        client.println(c);  
      }
    
    client.stop();
    delay(5000);
}
