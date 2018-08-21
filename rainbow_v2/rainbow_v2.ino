#include "FastLED.h"

#define NUM_LEDS 100
#define Y_LENGTH 9
#define X_LENGTH 12
#define DATA_PIN 11
#define TOP_LEFT_POT 3
#define TOP_RIGHT_POT 2
#define BOTTOM_LEFT_POT 0
#define INTERVAL 4

#define MIN_BRIGHTNESS 2

CRGB linearLeds[NUM_LEDS];
CHSV leds[Y_LENGTH][X_LENGTH];
uint16_t frameCounter = 0;

byte speedFactor = 1;
byte spread  = 10;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(linearLeds, NUM_LEDS);
  FastLED.setCorrection(LEDColorCorrection::TypicalSMD5050);
}

void loop() {
  FastLED.show();
  setBrightness();
  setSpeed();
  setSpread();
  
  frameCounter++;
  sweepingRainbow();
  transfer2dPixelsToLinear();

  FastLED.delay(INTERVAL);
}

void setBrightness() {
  float rawDitheredBrightness = analogRead(TOP_LEFT_POT) / 1023.0;
  byte ditheredBrightness = MIN_BRIGHTNESS + (pow(rawDitheredBrightness, 2) * (255.0 - MIN_BRIGHTNESS));
  FastLED.setBrightness(ditheredBrightness);
}

void setSpeed() {
  speedFactor = analogRead(BOTTOM_LEFT_POT) / 256;
  speedFactor = pow(2, speedFactor);
}

void setSpread() {
  byte step = analogRead(TOP_RIGHT_POT) / 171;
  switch(step) {
    case 0:
      spread = 1;
      break;
    case 1:
      spread = 2;
      break;
    case 2:
      spread = 3;
      break;
    case 3:
      spread = 5;
      break;
    case 4:
      spread = 7;
      break;
    case 5:
      spread = 10;
      break;
  }
}

void sweepingRainbow() {
  uint8_t startHue = (frameCounter % (256 * speedFactor)) / speedFactor;
  for (int y = 0; y < Y_LENGTH; y++) {
    uint8_t thisStartHue = startHue + (spread * y);
    fill_rainbow(leds[y], X_LENGTH, thisStartHue, spread);
  }
}

// based on my light jacket's LED arrangement
void transfer2dPixelsToLinear() {
  
  for (byte y = 0; y < Y_LENGTH; y++) {
    linearLeds[8 - y] = leds[y][0];
    linearLeds[9 + y] = leds[y][1];

    
    if (y < 7) {
      linearLeds[24 - y] = leds[y][2];
      linearLeds[25 + y] = leds[y][3];
    }
    
    linearLeds[40 - y] = leds[y][4];
    linearLeds[41 + y] = leds[y][5];
    linearLeds[58 - y] = leds[y][6];
    linearLeds[59 + y] = leds[y][7];
    
    if (y < 7) {
      linearLeds[74 - y] = leds[y][8];
      linearLeds[75 + y] = leds[y][9];
    }
    
    linearLeds[89 - y] = leds[y][10];
    linearLeds[90 + y] = leds[y][11];
  }
}
