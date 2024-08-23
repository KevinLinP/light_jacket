#include <emscripten.h>
#include "FastLED-3.5.0/src/FastLED_wasm.h"

#define WIDTH 9
#define HEIGHT 7
#define NUM_LEDS 63
#define MINIMUM_BRIGHTNESS 50

uint16_t num_leds_uint16;
CHSV leds[NUM_LEDS];
CRGB leds_rgb[NUM_LEDS];
// bool directions[NUM_LEDS];
// char speed[NUM_LEDS];
uint8_t persistence_of_version_color_channel;
uint8_t offset;

#define STRIP_DIVISIONS 5
#define STRIP_DIVISION_LENGTH 32
#define STRIP_LENGTH 192
// #define STRIP_DIVISION_MAX 21844 // 65535 / 3
#define STRIP_DIVISION_MAX 65535
uint16_t random_strip_values[STRIP_LENGTH];

void fill_random_strip_values()
{
  uint8_t i = 0;

  uint16_t last_division_value = 0;
  uint16_t last_value = 0;
  random_strip_values[i++] = last_division_value;

  for (int j = 0; j < STRIP_DIVISIONS; j++)
  {
    uint16_t next_division_value = random16(STRIP_DIVISION_MAX);
    int16_t step_size = (next_division_value - last_division_value) / STRIP_DIVISION_LENGTH;

    for (int k = 0; k < STRIP_DIVISION_LENGTH; k++)
    {
      last_value += step_size;
      random_strip_values[i++] = last_value;
    }

    last_division_value = next_division_value;

    // TODO: last case
  }

  uint16_t next_division_value = 0;
  int16_t step_size = (next_division_value - last_division_value) / STRIP_DIVISION_LENGTH;
  for (int k = 0; k < STRIP_DIVISION_LENGTH; k++)
  {
    last_value += step_size;
    random_strip_values[i++] = last_value;
  }
}

uint32_t get_millisecond_timer()
{
  return (uint32_t)emscripten_get_now();
}

extern "C"
{

  uint8_t getWidth()
  {
    return WIDTH;
  }

  uint8_t getHeight()
  {
    return HEIGHT;
  }

  CRGB *setup()
  {
    num_leds_uint16 = (uint16_t)NUM_LEDS;

    fill_random_strip_values();
    offset = 0;

    hsv2rgb_rainbow(leds, leds_rgb, NUM_LEDS);

    return leds_rgb;
  }

  void mask_persistence_of_vision()
  {
    if (persistence_of_version_color_channel == 0)
    {
      fadeUsingColor(leds_rgb, NUM_LEDS, CRGB(255, 0, 0));
      persistence_of_version_color_channel++;
    }
    else if (persistence_of_version_color_channel == 1)
    {
      fadeUsingColor(leds_rgb, NUM_LEDS, CRGB(0, 255, 0));
      persistence_of_version_color_channel++;
    }
    else if (persistence_of_version_color_channel == 2)
    {
      fadeUsingColor(leds_rgb, NUM_LEDS, CRGB(0, 0, 255));
      persistence_of_version_color_channel = 0;
    }
  }

  void loop()
  {
    uint8_t i = offset;
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      uint8_t value = random_strip_values[i] / 256;
      leds[x] = CHSV(0, 0, value);
      i++;
    }

    offset++;
    if (offset >= STRIP_LENGTH)
    {
      fill_random_strip_values();
      offset = 0;
    }

    // char i = offset_counter;

    // for (char y = 0; y < HEIGHT; y++)
    // {
    //   if (i >= STRIP_LENGTH - 1)
    //   {
    //     break;
    //   }
    //   u_int8_t value = random_strip_values[i] / 4;
    //   leds[y] = CHSV(0, 0, value);
    //   i += STRIP_DIVISION_LENGTH;
    // }

    // offset_counter += 1;
    // if (offset_counter > STRIP_LENGTH - 1)
    // {
    //   offset_counter = 0;
    // }

    hsv2rgb_rainbow(leds, leds_rgb, NUM_LEDS);
    // mask_persistence_of_vision();
  }
}

