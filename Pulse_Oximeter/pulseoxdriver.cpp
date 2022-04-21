#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <Wire.h>

MAX30105 particleSensor;

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

//Buffer for vitals to be transmitted on ESP8266
//Printed on Serial
char vitals_buff[18];





void setup()
{
  

  Serial.begin(115200); // initialize serial communication at 115200 bits per second:
  
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
  while (Serial.available() == 0) ; //wait until user presses a key
  Serial.read();

  byte ledBrightness = 51; //Options: 0=Off to 255=50mA
  byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

    
  vitals_buff[0]='H';
  vitals_buff[1]='R';
  vitals_buff[2]=':';
  vitals_buff[3]=' ';
  
  //Heart Rate data
  vitals_buff[4] = ' ';
  vitals_buff[5] = ' ';
  vitals_buff[6] = ' ';
  
  vitals_buff[7]=' ';
  
  vitals_buff[8]='S';
  vitals_buff[9]='P';
  vitals_buff[10]='O';
  vitals_buff[11]='2';
  vitals_buff[12]=':';
  vitals_buff[13]=' ';
  
  //SPO2 Data
  vitals_buff[14] = ' ';
  vitals_buff[15] = ' ';
  vitals_buff[16] = ' ';
  
  //End of buffer
  vitals_buff[17] = '\0';

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop()
{
  //How many bytes written to hr and spo2 buffers
  char hr_written[3];
  char spo2_written[3];

  
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps
  //read the first 100 samples, and determine the signal range
  Serial.print(F("Starting initial data collection.\n\n"));
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
//
//    Serial.print(F("red="));
//    Serial.print(redBuffer[i], DEC);
//    Serial.print(F(", ir="));
//    Serial.println(irBuffer[i], DEC);
  }
  //Serial.print(F("Initial data collection completed. Calculating initial vitals.\n\n"));

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  //Serial.print(F("Initial vitals calculated. Entering main loop\n\n"));
  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

    

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      //send samples and calculation result to terminal program through UART
//      Serial.print(F("red="));
//      Serial.print(redBuffer[i], DEC);
//      Serial.print(F(", ir="));
//      Serial.print(irBuffer[i], DEC);

      

    }
      if(validHeartRate){
        if(heartRate >= 100){
          itoa(heartRate, hr_written, 10);
          //Serial.print("HR ");
          //Serial.println(hr_written);
          vitals_buff[4] = hr_written[0];
          vitals_buff[5] = hr_written[1];
          vitals_buff[6] = hr_written[2];
        }else{
          itoa(heartRate, hr_written, 10);
          //Serial.print("hr ");
          //Serial.println( hr_written);
          vitals_buff[5] = hr_written[0];
          vitals_buff[6] = hr_written[1];
        }
      }
      
      if(validSPO2){
        
        if(spo2 == 100){
          
          vitals_buff[14] = '1';
          vitals_buff[15] = '0';
          vitals_buff[16] = '0';
        }else{
          itoa(spo2, spo2_written, 10);
          //Serial.print("SPO2 ");
          //Serial.println(spo2_written);
          vitals_buff[15] = spo2_written[0];
          vitals_buff[16] = spo2_written[1];
        }
      }
      Serial.println(vitals_buff);
      vitals_buff[4] = ' ';
      vitals_buff[5] = ' ';
      vitals_buff[6] = ' ';
      
      vitals_buff[14] = ' ';
      vitals_buff[15] = ' ';
      vitals_buff[16] = ' ';

      memset(hr_written, 0, sizeof(hr_written));
      memset(spo2_written, 0, sizeof(spo2_written));
    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }
}
