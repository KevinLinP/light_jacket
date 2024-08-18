#include <FastLED.h>

#define DATA_PIN 11
#define INTERVAL 10
#define TOP_LEFT_POT 3
#define MIN_DITHER_BRIGHTNESS 2

#define WIDTH 9
#define HEIGHT 7
#define NUM_LEDS 63
#define GREEN_HUE 95
#define RED_HUE 0
#define MIN_BRIGHTNESS 10

CHSV leds[NUM_LEDS];
CRGB leds_rgb[NUM_LEDS];
char speed[NUM_LEDS];
char persistence_of_version_color_channel;

uint8_t random_speed() {
  // return random8() % 2 + 2;
  return random8() % 2 + 1;
}

uint8_t random_value() {
  return random8() % 128 + 128;
}

uint8_t random_saturation() {
  return random8() % 32 + 224;
}

uint8_t random_hue() {
  if (random8() % 2 == 0) {
    return GREEN_HUE;
  } else {
    return RED_HUE;
  }
}

void random_fill() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(GREEN_HUE, random_saturation(), random_value());
    speed[i] = random_speed();
  }
}


uint8_t getWidth() {
  return WIDTH;
}

uint8_t getHeight() {
  return HEIGHT;
}

void setup() { 
  FastLED.addLeds<P9813, RGB>(leds_rgb, NUM_LEDS);

  random_fill();
  hsv2rgb_rainbow(leds, leds_rgb, NUM_LEDS);
}

void loop() {
  EVERY_N_MILLISECONDS(INTERVAL) {
    FastLED.show();

    for(int i = 0; i < NUM_LEDS; i++) {
      uint8_t previous_value = leds[i].value;
      leds[i].value -= speed[i];

      if (leds[i].value < MIN_BRIGHTNESS) {
        speed[i] = random_speed();
        leds[i].saturation = random_saturation();
        leds[i].value = random_value();
        leds[i].hue = random_hue();
      }
    }

    hsv2rgb_rainbow(leds, leds_rgb, NUM_LEDS);
  }

  EVERY_N_MILLISECONDS(250) {
    float rawDitheredBrightness = analogRead(TOP_LEFT_POT) / 1024.0;
    byte ditheredBrightness = MIN_DITHER_BRIGHTNESS + (pow(rawDitheredBrightness, 2) * (255.0 - MIN_DITHER_BRIGHTNESS));
    FastLED.setBrightness(ditheredBrightness);
  }
}


