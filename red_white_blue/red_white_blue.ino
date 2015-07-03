#include <SPI.h>
#include <TCL.h>

const int LEDS = 50;

const int FRAMES_PER_SECOND = 60;
const int TOTAL_FRAMES = 270;
const long INTERVAL = 1000 / FRAMES_PER_SECOND;
int frame = 0;

byte RED[] = {255, 0, 0};
byte WHITE[] = {220, 255, 255};
byte BLUE[] = {0, 0, 255};

long previousMillis = 0;


void setup() {
  TCL.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  
  float brightness = 0.01;
  byte newRed[] = {RED[0] * brightness, RED[1] * brightness, RED[2] * brightness};
  byte newWhite[] = {WHITE[0] * brightness, WHITE[1] * brightness, WHITE[2] * brightness};
  byte newBlue[] = {BLUE[0] * brightness, BLUE[1] * brightness, BLUE[2] * brightness};
  
  if (currentMillis - previousMillis > INTERVAL) {
    previousMillis = currentMillis;
    TCL.sendEmptyFrame();
    incrementFrame();

    int color = frame / 90;
    
    int n;
    for(n=0; n<LEDS; n++) {
      if(random(100000) < 250) {
        //byte intensity = random(123, 255);
        int i = n + color;
        if (i % 3 == 0) {
          sendColor(RED);
        } else if (i % 3 == 1) {
          byte mutedWhite[] = {100, 100, 100};
          sendColor(mutedWhite);
        } else {
          sendColor(BLUE);
        }
      } else {
        int i = n + color;
        if (i % 3 == 0) {
          sendColor(newRed);
        } else if (i % 3 == 1) {
          sendColor(newWhite);
        } else {
          sendColor(newBlue);
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
