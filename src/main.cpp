#include <Arduino.h>
#include "XeDoLine.h"

XeDoLine xe(
  2, 3, 4, 5, 6,   // cảm biến line
  7, 8, 9,         // động cơ trái
  10, 12, 11  , A0 , A1 )   ; 

void setup() {
  Serial.begin(9600);
  xe.batdau();
  xe.caiPID(1.0, 0.00001, 20.0);
  xe.tocdoban(80);
}

void loop() {
  xe.chay();
}
