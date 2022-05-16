//#include "MAX30105.h"
//#include "spo2_algorithm.h"
//#include "heartRate.h"
//#include <Wire.h>
//
//MAX30105 particleSensor;
//
//uint16_t irBuffer[75]; //infrared LED sensor data
//uint16_t redBuffer[75];  //red LED sensor data
//
//
//
//int32_t bufferLength; //data length
//int32_t spo2; //SPO2 value
//int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
//int32_t heartRate; //heart rate value
//int8_t validHeartRate; //indicator to show if the heart rate calculation is valid
//
////Buffer for vitals to be transmitted on ESP8266
////Printed on Serial
//char vitals_buff[18];
//
//
////Heart rate calculation variables
//const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
//byte rates[RATE_SIZE]; //Array of heart rates
//byte rateSpot = 0;
//long lastBeat = 0; //Time at which the last beat occurred
//
//float beatsPerMinute = 0;
//int beatAvg = 0;
//
//
//
//void setup()
//{
//  
//
//  Serial.begin(115200);
//  pinMode(7,INPUT);
//  Serial.println(F("Fudge"));
// 
//  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) 
//  {
//    
//    while (1);
//  }
//  //Wait until user is ready for input
//  //Serial.println("Press button to start data collection");
//  while(digitalRead(7) == LOW);
//
//  
//  byte ledBrightness = 200; //Options: 0=Off to 255=50mA
//  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
//  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
//  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
//  int pulseWidth = 411; //Options: 69, 118, 215, 411
//  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
//
//    
//  vitals_buff[0]='H';
//  vitals_buff[1]='R';
//  vitals_buff[2]=':';
//  vitals_buff[3]=' ';
//  
//  //Heart Rate data
//  vitals_buff[4] = ' ';
//  vitals_buff[5] = ' ';
//  vitals_buff[6] = ' ';
//  
//  vitals_buff[7]=' ';
//  
//  vitals_buff[8]='S';
//  vitals_buff[9]='P';
//  vitals_buff[10]='O';
//  vitals_buff[11]='2';
//  vitals_buff[12]=':';
//  vitals_buff[13]=' ';
//  
//  //SPO2 Data
//  vitals_buff[14] = ' ';
//  vitals_buff[15] = ' ';
//  vitals_buff[16] = ' ';
//  
//  //End of buffer
//  vitals_buff[17] = '\n';
//
//  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
//}
//
//void loop()
//{
//  //How many bytes written to hr and spo2 buffers
//  char hr_written[3];
//  char spo2_written[3];
//  //Time between peeks
//  long delta;
// 
//  
//  bufferLength = 75; //buffer length of 100 stores 4 seconds of samples running at 25sps
//  //read the first 100 samples, and determine the signal range
//  //Serial.print(F("Starting initial data collection.\n\n"));
//  for (byte i = 0 ; i < bufferLength ; i++)
//  {
//    while (particleSensor.available() == false) //do we have new data?
//      particleSensor.check(); //Check the sensor for new data
//
//    redBuffer[i] = particleSensor.getRed();
//    irBuffer[i] = particleSensor.getIR();
//    particleSensor.nextSample();
//  }
//
//
//
//  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
//  while (1)
//  {
//    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
//    for (byte i = 25; i < 75; i++)
//    {
//      redBuffer[i - 25] = redBuffer[i];
//      irBuffer[i - 25] = irBuffer[i];
//    }
//
//    //take 25 sets of samples before calculating the heart rate.
//    for (byte i = 50; i < 75; i++)
//    {
//      while (particleSensor.available() == false) //do we have new data?
//        particleSensor.check(); //Check the sensor for new data
//
//    
//
//      redBuffer[i] = particleSensor.getRed();
//      irBuffer[i] = particleSensor.getIR();
//     if (checkForBeat(irBuffer[i]) == true)
//      {
//        //We sensed a beat!
//        delta = millis() - lastBeat;
//        lastBeat = millis();
//    
//        beatsPerMinute = 60 / (delta / 1000.0);
//    
//        if (beatsPerMinute < 255 && beatsPerMinute > 20)
//        {
//          rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
//          rateSpot %= RATE_SIZE; //Wrap variable
//    
//          //Take average of readings
//          beatAvg = 0;
//          for (byte x = 0 ; x < RATE_SIZE ; x++)
//            beatAvg += rates[x];
//          beatAvg /= RATE_SIZE;
//        }
//      }
//      
//      particleSensor.nextSample(); //We're finished with this sample so move to next sample
//
//      
//
//    }
//      if(validHeartRate){
//        if(heartRate >= 100){
//          itoa(heartRate, hr_written, 10);
//          vitals_buff[4] = hr_written[0];
//          vitals_buff[5] = hr_written[1];
//          vitals_buff[6] = hr_written[2];
//        }else{
//          itoa(heartRate, hr_written, 10);
//          vitals_buff[5] = hr_written[0];
//          vitals_buff[6] = hr_written[1];
//        }
//      }
//      
//      if(validSPO2){
//        
//        if(spo2 == 100){
//          
//          vitals_buff[14] = '1';
//          vitals_buff[15] = '0';
//          vitals_buff[16] = '0';
//        }else{
//          itoa(spo2, spo2_written, 10);
//          //Serial.print("SPO2 ");
//          //Serial.println(spo2_written);
//          vitals_buff[15] = spo2_written[0];
//          vitals_buff[16] = spo2_written[1];
//        }
//      }
//      Serial.write("R");
//      Serial.write(vitals_buff, sizeof(vitals_buff));
//      Serial.print(F("Better HR: "));
//      Serial.print(beatsPerMinute);
//      Serial.print(F(" Better avg: "));
//      Serial.println(beatAvg); 
//      vitals_buff[4] = ' ';
//      vitals_buff[5] = ' ';
//      vitals_buff[6] = ' ';

//      
//      vitals_buff[14] = ' ';
//      vitals_buff[15] = ' ';
//      vitals_buff[16] = ' ';
//
//      memset(hr_written, 0, sizeof(hr_written));
//      memset(spo2_written, 0, sizeof(spo2_written));
//    //After gathering 25 new samples recalculate HR and SP02
//    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
//  }
//}

#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor; // initialize MAX30102 with I2C

void setup()
{
  Serial.begin(115200);
  while(!Serial); //We must wait for Teensy to come online
  delay(100);
  Serial.println("");
  Serial.println("MAX30102");
  Serial.println("");

  pinMode(7, INPUT);
  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  while(digitalRead(7) == LOW);

  byte ledBrightness = 70; //Options: 0=Off to 255=50mA
  byte sampleAverage = 32; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 69; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
  

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop() {
  particleSensor.check(); //Check the sensor
  char data_buf[512] = {0};
  int amt_to_send = 0;
  unsigned long micro_sec;
  uint32_t ir, red;
  while (particleSensor.available()) {
    micro_sec = micros();
    ir = particleSensor.getFIFOIR();
    red = particleSensor.getFIFORed();
    amt_to_send = sprintf(data_buf, "%ld:%ld,%ld\n", micro_sec, ir, red);
    Serial.print(data_buf);
    Serial.flush();
    // read next set of samples
    particleSensor.nextSample();      
  }
}
