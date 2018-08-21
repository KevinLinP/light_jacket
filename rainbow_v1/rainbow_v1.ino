#include "FastLED.h"

#define NUM_LEDS 100
#define ROWS 9
#define COLUMNS 12
#define DATA_PIN 11
#define TOP_LEFT_POT 3
#define TOP_RIGHT_POT 2
#define BOTTOM_LEFT_POT 0
#define INTERVAL 17

#define MIN_BRIGHTNESS 2

CRGB linearLeds[NUM_LEDS];

byte columnHues[COLUMNS];
CRGB leds[ROWS][COLUMNS];

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(linearLeds, NUM_LEDS);

    float hueInterval = 256.0 / COLUMNS;
    for(byte column = 0; column < COLUMNS; column++) {
      columnHues[column] = hueInterval * column;
    }
}

byte flashLoop = 0;
byte frameCounter = 0;
byte exceptionCounter = 0;
byte state = 0;

void loop() {
  FastLED.show();

  float rawDitheredBrightness = analogRead(TOP_LEFT_POT) / 1024.0;
  byte ditheredBrightness = MIN_BRIGHTNESS + (pow(rawDitheredBrightness, 2) * (255.0 - MIN_BRIGHTNESS));
  FastLED.setBrightness(ditheredBrightness);

  float rawColorBrightness = analogRead(TOP_RIGHT_POT) / 1024.0;
  byte colorBrightness = pow(rawColorBrightness, 2) * 255.0;

  float rawFlashRate = analogRead(BOTTOM_LEFT_POT) / 1024.0;
  byte flashRate = 10 + (pow(rawFlashRate, 3) * 245);

  fillRainbowBase(frameCounter, colorBrightness);

  transfer2dPixelsToLinear();

  FastLED.delay(INTERVAL);
}

void fillRainbowBase(byte frameCounter, byte colorBrightness) {
  for(byte column = 0; column < COLUMNS; column++) {
    //columnHues[column] += sin8(frameCounter + (column * (256.0 / COLUMNS))) / 255.0 * 5 + 1;
    columnHues[column] += sin8(frameCounter * column) / 255.0 * 4 + 0;

    byte hue = columnHues[column];
    CHSV hsvColor(hue, 255, colorBrightness);
    CRGB rgbColor;
    hsv2rgb_rainbow(hsvColor, rgbColor);

    for(byte row = 0; row < ROWS; row++) {
      leds[row][column] = rgbColor;
    }
  }
}

byte randomState() {
  return 1;
  
  byte randomNum = random8();

  if (randomNum < 25) {
    return 2;
  } else {
    return 1;
  }
}

// based on my light jacket's LED arrangement
void transfer2dPixelsToLinear() {
  
  for (byte y = 0; y < ROWS; y++) {
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

