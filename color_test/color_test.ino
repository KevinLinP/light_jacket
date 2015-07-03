#include <SPI.h>
#include <TCL.h>

const int LEDS = 50;

void setup() {
  TCL.begin();
}

void loop() {
  TCL.sendEmptyFrame();
  
  int n;
  for(n = 0; n < LEDS; n++) {
    TCL.sendColor(130, 130, 130);
  }
  TCL.sendEmptyFrame();

  delay(10000);
}
