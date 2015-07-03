#include <SPI.h>
#include <TCL.h>

const int LEDS = 50;

const int TOTAL_FRAMES = 3;
const int FRAMES_PER_SECOND = 1;
const long INTERVAL = 1000 / FRAMES_PER_SECOND;
int frame = 0;

long previousMillis = 0;


void setup() {
  TCL.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > INTERVAL) {
    previousMillis = currentMillis;
    TCL.sendEmptyFrame();
    incrementFrame();

    int n;
    
    for(n=0; n<LEDS; n++) {
      int i = n + frame;
      if (i % 3 == 0) {
        TCL.sendColor(255, 0, 0);
      } else if (i % 3 == 1) {
        TCL.sendColor(90, 255, 255);
      } else {
        TCL.sendColor(0, 0, 255);
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
