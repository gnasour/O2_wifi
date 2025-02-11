#include <Wire.h>
#include "MAX30105.h"
#include <LiquidCrystal.h>

MAX30105 particleSensor; // initialize MAX30102 with I2C
unsigned long init_micro_sec;

const int rs = 11, en = 10, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(115200);
  while(!Serial); //We must wait for Teensy to come online
  delay(100);
  Serial.println("");
  Serial.println("MAX30102 getting ready...");
  Serial.println("");
  lcd.begin(16,2);
  lcd.print("Hello, World");
  lcd.setCursor(0,1);
  lcd.print("bPO2 monitor");
  

  pinMode(7, INPUT);
  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  while(digitalRead(7) == LOW);

  byte ledBrightness = 80; //Options: 0=Off to 255=50mA
  byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
  
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  init_micro_sec = micros();
}

void loop() {

  particleSensor.check();
  
  //Buffer allocation for data sent to Serial -> Esp -> WiFi -> Server
  char data_buf[512] = {0};
  int amt_to_send = 0;
  
  //Use this value to subtract current micros from initial micros of the system
  unsigned long delta_micro_sec;
  
  uint32_t ir, red;

  //Main loop, while PPG sensor has data available continue with data collection
  while (particleSensor.available()) {
    delta_micro_sec = micros() - init_micro_sec;
    ir = particleSensor.getFIFOIR();
    red = particleSensor.getFIFORed();
    amt_to_send = sprintf(data_buf, "%ld:%ld,%ld\n", delta_micro_sec, ir, red);
    Serial.write(data_buf, amt_to_send);
    // read next set of samples
    particleSensor.nextSample();      
  }
}
