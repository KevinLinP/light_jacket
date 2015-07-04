#include <SPI.h>
#include <TCL.h>

const byte LEDS = 50;

const byte FRAMES_PER_SECOND = 60;
const long INTERVAL = 1000 / FRAMES_PER_SECOND;
const float MAX_BRIGHTNESS = 0.5;

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

float brightness;
byte tempColor[3];

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
      brightness = MAX_BRIGHTNESS * ledLifeLeft[i]  / ledLifetime[i];
      ledLifeLeft[i]--;
      
      switch(ledColor[i]) {
        case RED :
        changeBrightness(FULL_RED, brightness, tempColor);
        break;
        case WHITE :
        changeBrightness(FULL_WHITE, brightness, tempColor);
        break;
        case BLUE :
        changeBrightness(FULL_BLUE, brightness, tempColor);
        break;
      }
      sendColor(tempColor);

      if (ledLifeLeft[i] == 0) {
        ledColor[i] = random(0, 3);
        byte lifetime = random(MIN_LIFETIME, MAX_LIFETIME);
        ledLifetime[i] = lifetime;
        ledLifeLeft[i] = lifetime;
      } 
    }
   
    
    TCL.sendEmptyFrame();
  }
}

void sendColor(byte color[3]) {
  TCL.sendColor(color[0], color[1], color[2]);
}

void changeBrightness(byte oldColor[3], float brightness, byte newColor[3]) {
  newColor[0] = brightness * oldColor[0];
  newColor[1] = brightness * oldColor[1];
  newColor[2] = brightness * oldColor[2];
}

