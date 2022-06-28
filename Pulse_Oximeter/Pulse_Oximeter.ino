#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>


#define BLUE_LED 2
 
const uint16_t port = 1025;
const char *host = "192.168.1.168";
WiFiClient client;
   
 void setup()
 {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin("Nasour", "Hn4281992@#"); // change it to your ussid and password
    pinMode(BLUE_LED, OUTPUT);
    
    while(WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(BLUE_LED, LOW);
      delay(2500);
      digitalWrite(BLUE_LED, HIGH);
      delay(2500);
    }
      digitalWrite(BLUE_LED, HIGH);
      Serial.print("Hi");
    
 }

 void loop()
 {
  unsigned int buff_size = 0;
  char buff[256];
   
    while (!client.connect(host, port))
    {
       digitalWrite(BLUE_LED, LOW);
       delay(250);
       digitalWrite(BLUE_LED, HIGH);
       delay(250);
    }
      digitalWrite(BLUE_LED, HIGH);
      
    //Clear the serial buffer before sending data to the client
    while(Serial.available()){
      Serial.read();
    }
     while (client.connected())
    {
      if(Serial.available()){
        *(buff+buff_size) = Serial.read();
        buff_size++;
        if(*(buff+buff_size-1) == '\n'){
          client.write(buff,buff_size);
          memset(buff, 0, buff_size);
          buff_size = 0;
        }
        
     } 
    } 
    client.stop();
 }

//void setup(){}
//void loop(){}
