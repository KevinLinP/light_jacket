#include <SPI.h>
#include <TCL.h>

const int LEDS = 50;

const int FRAMES_PER_SECOND = 60;
const int TOTAL_FRAMES = 270;
const long INTERVAL = 1000 / FRAMES_PER_SECOND;
int frame = 0;

byte FULL_RED[] = {255, 0, 0};
byte FULL_WHITE[] = {220, 255, 255};
byte FULL_BLUE[] = {0, 0, 255};

long previousMillis = 0;


void setup() {
  TCL.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  
  float brightness = 0.01;
  
  byte red[3];
  byte white[3];
  byte blue[3];
  changeBrightness(FULL_RED, brightness, red);
  changeBrightness(FULL_WHITE, brightness, white);
  changeBrightness(FULL_BLUE, brightness, blue);

  byte flashWhite[3];
  changeBrightness(FULL_WHITE, 0.33333, flashWhite);
  
  
  if (currentMillis - previousMillis > INTERVAL) {
    previousMillis = currentMillis;
    TCL.sendEmptyFrame();
    incrementFrame();

    int color = frame / 90;
    
    int n;
    for(n=0; n<LEDS; n++) {
      if(random(100000) < 250) {
        byte randomBrightness = random(5, 11);
        float brightness = randomBrightness / 10;
        int i = n + color;
        
  
        if (i % 3 == 0) {
          sendColor(FULL_RED);
        } else if (i % 3 == 1) {
          sendColor(flashWhite);
        } else {
          sendColor(FULL_BLUE);
        }
      } else {
        int i = n + color;
        if (i % 3 == 0) {
          sendColor(red);
        } else if (i % 3 == 1) {
          sendColor(white);
        } else {
          sendColor(blue);
        }
      }
    }
    
    TCL.sendEmptyFrame();
  }
}

void incrementFrame() {
  frame++;
  if (frame == TOTAL_FRAMES) {
    frame = 0;
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

