#include "FastLED/FastLED.h"

#include <stdio.h>

#define Y_LENGTH 9
#define X_LENGTH 12

extern "C" {

CHSV leds[Y_LENGTH][X_LENGTH];
uint8_t frameCounter8 = 0;
uint16_t frameCounter = 0;

/*
void sweepingRainbow() {
  uint8_t startHue = (frameCounter % 256);
  for (int y = 0; y < Y_LENGTH; y++) {
    uint8_t thisStartHue = startHue + (10 * y);

    fill_rainbow(leds[y], X_LENGTH, thisStartHue, 10);
  }
}
*/

void rotatingRainbow() {
  const uint8_t HUE_VARIANCE = 25;
  uint8_t angle = (frameCounter % 4096) / 16;
  uint8_t startHue = (frameCounter % 512) / 2;
  float yHueDist = ((-128.0 + sin8(angle)) / 128.0) * HUE_VARIANCE;
  float xHueDist = ((-128.0 + cos8(angle)) / 128.0) * HUE_VARIANCE;

  for (int y = 0; y < Y_LENGTH; y++) {
    uint8_t thisStartHue = startHue + (yHueDist * y);

    for (int x = 0; x < X_LENGTH; x++) {
      uint8_t thisHue = thisStartHue + (xHueDist * x);
      leds[y][x] = CHSV(thisHue, 255, 255);
    }
  }
}

/*void blankLine(uint8_t speedReduction, bool down) {*/
  /*uint8_t lineNum = (frameCounter8 % (Y_LENGTH * speedReduction) / speedReduction);*/
  /*if (!down) {*/
    /*lineNum = (Y_LENGTH - 1) - lineNum;*/
  /*}*/

  /*for (int x = 0; x < X_LENGTH; x++) {*/
    /*CRGB previousColor = leds[lineNum][x];*/

    /*leds[lineNum][x] = CHSV(0, 0, 0);*/
  /*}*/
/*}*/

void blankLine2(uint8_t speedReduction) {
  uint8_t speedFactor = 4;
  uint8_t divisor = (256 / speedFactor); // 32
  uint8_t lineNum = (frameCounter / divisor) % 8;
  uint8_t closeness = (frameCounter % divisor); // 0 - 63

  uint8_t closenessRange = divisor / 2; // 16
  if (closeness > closenessRange) {
    closeness = divisor - closeness;
  }
  closeness = closeness * (256 / (closenessRange));
  closeness = 256 - closeness;
  /*closeness = dim8_video(closeness);*/

  uint8_t newVal = scale8_video(255, closeness);

  for (int x = 0; x < X_LENGTH; x++) {
    CHSV current = leds[lineNum][x];
    leds[lineNum][x] = CHSV(current.hue, current.sat, newVal);
  }
}

/*void primaryFilter() {*/
  /*uint8_t colorInt = frameCounter % 3;*/
  /*bool isRed = (colorInt == 0);*/
  /*bool isGreen = (colorInt == 1);*/

  /*for (int y = 0; y < Y_LENGTH; y++) {*/
    /*for (int x = 0; x < X_LENGTH; x++) {*/
      /*CRGB previousColor = leds[y][x];*/

      /*if (isRed) {*/
        /*previousColor.g = 0;*/
        /*previousColor.b = 0;*/
      /*} else if (isGreen) {*/
        /*previousColor.r = 0;*/
        /*previousColor.b = 0;*/
      /*} else {*/
        /*previousColor.r = 0;*/
        /*previousColor.g = 0;*/
      /*}*/

      /*leds[y][x] = previousColor; // converting again?*/
    /*}*/
  /*}*/
/*}*/

void generateFrame() {
  rotatingRainbow();
  /*blankLine(16, true);*/
  blankLine2(16);
  /*blankLine(8, false);*/
  /*blankLine(4, false);*/
}

void translateToLinear(uint8_t *array) {
  int i = 0;

  for (int y = 0; y < Y_LENGTH; y++) {
    for (int x = 0; x < X_LENGTH; x++) {
      CRGB led = leds[y][x]; // converting here!

      array[i++] = led.r;
      array[i++] = led.g;
      array[i++] = led.b;
    }
  }
}

void loopAndPopulateArray(uint8_t *array) {
  frameCounter++;
  frameCounter8++;
  generateFrame();
  translateToLinear(array);
}

}
