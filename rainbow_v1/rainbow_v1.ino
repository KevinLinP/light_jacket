#include "FastLED.h"

#define NUM_LEDS 50
#define ROWS 10
#define COLUMNS 6
#define DATA_PIN 1
#define TOP_LEFT_POT 3
#define TOP_RIGHT_POT 2
#define INTERVAL 17

#define MIN_BRIGHTNESS 2

CRGB linearLeds[NUM_LEDS];

byte columnHues[COLUMNS];
CRGB leds[ROWS][COLUMNS];

byte flashLoop = COLUMNS;

void setup() {
    FastLED.addLeds<P9813, RGB>(linearLeds, NUM_LEDS);

    float hueInterval = 256.0 / COLUMNS;
    for(byte column = 0; column < COLUMNS; column++) {
      columnHues[column] = hueInterval * column;
    }
}



void loop() {
  FastLED.show();

  float rawDitheredBrightness = (analogRead(TOP_LEFT_POT) >> 2) / 255.0;
  byte ditheredBrightness = MIN_BRIGHTNESS + (pow(rawDitheredBrightness, 2) * (255.0 - MIN_BRIGHTNESS));
  FastLED.setBrightness(ditheredBrightness);

  float rawColorBrightness = (analogRead(TOP_RIGHT_POT) >> 2) / 255.0;
  byte colorBrightness = pow(rawColorBrightness, 2) * 255.0;

  flashLoop++;

  for(byte column = 0; column < COLUMNS; column++) {
    columnHues[column] += 1;

    CHSV hsvColor(columnHues[column], 255, colorBrightness);
    CRGB rgbColor;
    hsv2rgb_rainbow(hsvColor, rgbColor);

    for(byte row = 0; row < ROWS; row++) {
      if (row == flashLoop) {
        leds[row][column].setHSV(0, 0, 255);
      } else {
        leds[row][column] = rgbColor;
      }
    }
  }

  transfer2dPixelsToLinear();

  FastLED.delay(INTERVAL);
}

// based on my light jacket's LED arrangement
// it'd be nice if GCC unrolled these loops
void transfer2dPixelsToLinear() {
  
  for (byte row = 0; row < 5; row++) {
    linearLeds[0 + row] = leds[1 + row][0];
  }

  for (byte row = 0; row < 4; row++) {
    linearLeds[8 - row] = leds[2 + row][1];
  }

  for (byte row = 0; row < 4; row++) {
    linearLeds[9 + row] = leds[2 + row][2];
  }

  for (byte row = 0; row < 6; row++) {
    linearLeds[18 - row] = leds[row][3];
  }

  for (byte row = 0; row < 6; row++) {
    linearLeds[19 + row] = leds[row][4];
  }

  for (byte row = 0; row < 6; row++) {
    linearLeds[30 - row] = leds[row][5];
  }
  
  for (byte row = 0; row < 6; row++) {
    linearLeds[31 + row] = leds[row][6];
  }
  
  for (byte row = 0; row < 4; row++) {
    linearLeds[40 - row] = leds[2 + row][7];
  }
  
  for (byte row = 0; row < 4; row++) {
    linearLeds[41 + row] = leds[2 + row][8];
  }

  for (byte row = 0; row < 5; row++) {
    linearLeds[49 - row] = leds[1 + row][9];
  }
}
