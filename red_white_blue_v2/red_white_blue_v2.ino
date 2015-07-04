#include <SPI.h>
#include <TCL.h>

const byte LEDS = 50;

const byte FRAMES_PER_SECOND = 60;
const long INTERVAL = 1000 / FRAMES_PER_SECOND;
const float MAX_BRIGHTNESS = 0.125;

const byte RED = 0;
const byte WHITE = 1;
const byte BLUE = 2;
const byte MIN_LIFETIME = 2 * FRAMES_PER_SECOND;
const byte MAX_LIFETIME = 3 * FRAMES_PER_SECOND;

byte FULL_RED[] = {255, 0, 0};
byte FULL_WHITE[] = {255, 255, 255};
byte FULL_BLUE[] = {0, 0, 255};

long previousMillis = 0;

byte ledColor[LEDS];
byte ledLifetime[LEDS];
byte ledLifeLeft[LEDS];

void setup() {
  TCL.begin();

  byte i;
  for(i = 0; i < LEDS; i++) {
    ledColor[i] = random(0, 3);
    byte lifetime = random(MIN_LIFETIME, MAX_LIFETIME);
    ledLifetime[i] = lifetime;
    ledLifeLeft[i] = random(0, lifetime);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis > INTERVAL) {
    previousMillis = currentMillis;
    TCL.sendEmptyFrame();

    byte i;
    for(i = 0; i < LEDS; i++) {
      byte color = ledColor[i];
      byte lifeLeft = ledLifeLeft[i];

      float brightness;
      if (random(100000) < 125) {
        sendColor(FULL_WHITE);
      } else {
        brightness = 1.0 * lifeLeft  / ledLifetime[i] * MAX_BRIGHTNESS;

        switch(color) {
        case RED :
        sendColorAndBrightness(FULL_RED, brightness);
        break;
        case WHITE :
        sendColorAndBrightness(FULL_WHITE, brightness);
        break;
        case BLUE :
        sendColorAndBrightness(FULL_BLUE, brightness);
        break;
        }
      }

      if (lifeLeft == 0) {
        ledColor[i] = (color + random(1, 3)) % 3;
        byte lifetime = random(MIN_LIFETIME, MAX_LIFETIME);
        ledLifetime[i] = lifetime;
        ledLifeLeft[i] = lifetime;
      } else {
        ledLifeLeft[i]--;
      }
    }
   
    
    TCL.sendEmptyFrame();
  }
}

void sendColor(byte color[3]) {
  TCL.sendColor(color[0], color[1], color[2]);
}

void sendColorAndBrightness(byte color[3], float brightness) {
  TCL.sendColor(color[0] * brightness, color[1] * brightness, color[2] * brightness);
}
