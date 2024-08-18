#include <emscripten.h>
#include "FastLED-3.5.0/src/FastLED_wasm.h"

#define WIDTH 9
#define HEIGHT 7
#define NUM_LEDS 63
#define GREEN_HUE 120
#define RED_HUE 0
#define MINIMUM_BRIGHTNESS 50

uint16_t num_leds_uint16;
CHSV leds[NUM_LEDS];
CRGB leds_rgb[NUM_LEDS];
bool directions[NUM_LEDS];
char speed[NUM_LEDS];
char persistence_of_version_color_channel;

uint8_t random_speed() {
  return random8() % 2 + 2;
  // return random8() % 3 + 1;
}

uint8_t random_value() {
  return random8() / 4 + 192;
}

uint8_t random_saturation() {
  return random8() % 128 + 128;
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
    directions[i] = random8() >= 128;
    speed[i] = random_speed();
  }
}

uint32_t get_millisecond_timer() {
  return (uint32_t) emscripten_get_now();
}

extern "C" {

  uint8_t getWidth() {
    return WIDTH;
  }

  uint8_t getHeight() {
    return HEIGHT;
  }

  CRGB * setup() { 
    num_leds_uint16 = (uint16_t) NUM_LEDS;
    random_fill();

    hsv2rgb_rainbow(leds, leds_rgb, NUM_LEDS);

    return leds_rgb;
  }

  void mask_persistence_of_vision() {
    if (persistence_of_version_color_channel == 0) {
      // fadeUsingColor(leds_rgb, NUM_LEDS, CRGB(255, 0, 0));
      persistence_of_version_color_channel++;
    } else if (persistence_of_version_color_channel == 1) {
      // fadeUsingColor(leds_rgb, NUM_LEDS, CRGB(0, 255, 0));
      persistence_of_version_color_channel++;
    } else if (persistence_of_version_color_channel == 2) {
      // fadeUsingColor(leds_rgb, NUM_LEDS, CRGB(0, 0, 255));
      persistence_of_version_color_channel = 0;
    }
  }

  void loop() {
    if (persistence_of_version_color_channel == 0) {
      for(int i = 0; i < NUM_LEDS; i++) {
        uint8_t previous_value = leds[i].value;
        leds[i].value -= speed[i];

        if (leds[i].value < MINIMUM_BRIGHTNESS) {
          speed[i] = random_speed();
          leds[i].saturation = random_saturation();
          leds[i].value = random_value();
          leds[i].hue = random_hue();
        }
      }
    }
    hsv2rgb_rainbow(leds, leds_rgb, NUM_LEDS);

    mask_persistence_of_vision();
  }
}

