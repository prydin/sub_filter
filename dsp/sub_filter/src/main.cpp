#include <Arduino.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "filter_biquad_f.h"
#include "filterbank.h"

#define FQ_PIN 27
#define DELAY_PIN 26  

AudioInputI2S            i2s1;          
AudioFilterBiquadFloat   biquad1;       
AudioEffectDelay         delay1;
AudioOutputI2S           i2s2;          
AudioConnection          patchCord1(i2s1, 0, biquad1, 0);
AudioConnection          patchCord2(biquad1, 0, delay1, 0);
AudioConnection          patchCord3(delay1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     


void setup() {
  Serial.begin(9600);
  // Initialize audio system
  AudioMemory(100);
  sgtl5000_1.enable(0, 8000);
  sgtl5000_1.volume(0.5); // Set volume to 50%
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // Select line-in as input
  biquad1.setSosCoefficients(2, filterBank); 
  delay1.delay(0, 0); // Set delay to 100ms
  pinMode(FQ_PIN, INPUT_DISABLE);
  pinMode(DELAY_PIN, INPUT_DISABLE);
}

void loop() {
  delay(100);
  pinMode(FQ_PIN, INPUT_DISABLE);
  Serial.println(analogRead(FQ_PIN));
  int fc = (analogRead(FQ_PIN) * 200) / 1023; // Scale to 0-200
  if(fc > 199) {
    fc = 199; // Limit frequency to 200
  }
  biquad1.setSosCoefficients(2, filterBank + fc * 2 * STAGE_COEFFICIENTS);
  int delay = analogRead(DELAY_PIN) * 100 / 1023; 
  delay1.delay(0, delay); 
}
