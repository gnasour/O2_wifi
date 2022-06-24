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

  byte ledBrightness = 100; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 69; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
  
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop() {

  particleSensor.check();
  
  //Buffer allocation for data sent to Serial -> Esp -> WiFi -> Server
  char data_buf[512] = {0};
  int amt_to_send = 0;
  
  //Initial microsecond of system when data collection starts
  unsigned long init_micro_sec = micros();
  //Use this value to subtract current micros from initial micros of the system
  unsigned long delta_micro_sec = 0;
  
  uint32_t ir, red;

  //Main loop, while PPG sensor has data available continue with data collection
  while (particleSensor.available()) {
    delta_micro_sec = micros() - init_micro_sec;
    ir = particleSensor.getFIFOIR();
    red = particleSensor.getFIFORed();
    amt_to_send = sprintf(data_buf, "%ld:%ld,%ld\n", micro_sec, ir, red);
    Serial.write(data_buf, amt_to_send);
    // read next set of samples
    particleSensor.nextSample();      
  }
}
