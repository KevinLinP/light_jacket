#include "FastLED.h"

#define NUM_LEDS 50
#define Y_LENGTH 5
#define X_LENGTH 10
#define DATA_PIN 11
#define TOP_LEFT_POT 3
#define TOP_RIGHT_POT 2
#define BOTTOM_LEFT_POT 0
#define INTERVAL 8

#define MIN_BRIGHTNESS 2

CRGB linearLeds[NUM_LEDS];
CHSV leds[Y_LENGTH][X_LENGTH];
uint16_t frameCounter = 0;

byte speedFactor = 1;
byte spread  = 10;

void setup() {
  FastLED.addLeds<P9813, RGB>(linearLeds, NUM_LEDS);
  //FastLED.setCorrection(LEDColorCorrection::TypicalSMD5050);
}

void loop() {
  FastLED.show();
  
  setBrightness();
  setSpeed();
  setSpread();
  
  frameCounter++;
  //rotatingRainbow();
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
  for (uint8_t y = 0; y < Y_LENGTH; y++) {
    uint8_t thisStartHue = startHue + (spread * y);
    fill_rainbow(leds[y], X_LENGTH, thisStartHue, spread);
  }
}

void rotatingRainbow() {
  uint8_t angle = (frameCounter % 1024) / 4;
  uint8_t startHue = (frameCounter % 512) / 2;
  uint8_t deltaDivisor = 8;
  /*startHue = 0;*/
  uint8_t xHueDelta = triwave8(angle); // why the F triwave works better, i'm not sure.
  /*xHueDelta = 0;*/
  uint8_t yHueDelta = triwave8(angle) + 128;
  /*yHueDelta = 0;*/
  
  // i'm sure when someone tells me the easier way to do this i'll be embarassed
  if (yHueDelta >= 128) {
    yHueDelta = 255 - yHueDelta;
  }
  yHueDelta /= deltaDivisor;
  
  if (xHueDelta >= 128) {
    xHueDelta = 255 - xHueDelta;
  }
  xHueDelta /= deltaDivisor;

  CHSV rowStartColors[Y_LENGTH];

  if (angle >= 64 && angle <= 191) {
    fill_rainbow(rowStartColors, Y_LENGTH, startHue, yHueDelta);
  } else {
    CHSV columnToReverse[Y_LENGTH];
    fill_rainbow(columnToReverse, Y_LENGTH, startHue, yHueDelta);
    for (uint8_t i = 0; i < Y_LENGTH; i++) {
      rowStartColors[Y_LENGTH - 1 - i] = columnToReverse[i];
    }
  }

  for (int y = 0; y < Y_LENGTH; y++) {
    CHSV rowStartColor = rowStartColors[y];

    if (angle <= 127) {
      fill_rainbow(leds[y], X_LENGTH, rowStartColor.hue, xHueDelta);
    } else {
      CHSV rowToReverse[X_LENGTH];
      fill_rainbow(rowToReverse, X_LENGTH, rowStartColor.hue, xHueDelta);
      for (uint8_t i = 0; i < X_LENGTH; i++) {
        leds[y][X_LENGTH - 1 - i] = rowToReverse[i];
      }
    }
  }
}


// based on my light jacket's LED arrangement
void transfer2dPixelsToLinear() {
  
  for (byte y = 0; y < Y_LENGTH; y++) {
    linearLeds[4 - y] = leds[y][0];
    linearLeds[5 + y] = leds[y][1];
    
    linearLeds[14 - y] = leds[y][2];
    linearLeds[15 + y] = leds[y][3];
    
    linearLeds[24 - y] = leds[y][4];
    linearLeds[25 + y] = leds[y][5];
    
    linearLeds[34 - y] = leds[y][6];
    linearLeds[35 + y] = leds[y][7];

    linearLeds[44 - y] = leds[y][8];
    linearLeds[45 + y] = leds[y][9];
  }
}
