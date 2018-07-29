//sudo ./esptool.py  write_flash 0x00000 /tmp/arduino_build_66120/main.ino.bin
#include "RobotGFX.h" // graphics library
#include "FS.h" // SPIFFS file system
#include "animations.h" 
#include "scrolls.h"
#include "bitmaps.h"


// These are our keys
#define PIN_LEFT    0
#define PIN_DOWN    16
#define PIN_RIGHT   15
#define PIN_UP      13
#define PIN_A       12
#define PIN_B       14

// Key press detection macros. May need to be debounced. 
#define PIN_LEFT_PRESS() (digitalRead(PIN_LEFT) == LOW)
#define PIN_DOWN_PRESS() (digitalRead(PIN_DOWN) == LOW)
#define PIN_RIGHT_PRESS() (digitalRead(PIN_RIGHT) == HIGH)
#define PIN_UP_PRESS() (digitalRead(PIN_UP) == LOW)
#define PIN_A_PRESS() (digitalRead(PIN_A) == LOW)
#define PIN_B_PRESS() (digitalRead(PIN_B) == LOW)


// create a global frame buffer object. 
AGFXShim frame_buffer(SDA_PIN,SCL_PIN);


/*
 * A simple "Knight rider" animation using  
 * the adafruit graphics library. Note how we write 
 * to the frame buffer but that it doesn't draw the results until
 * we call execute.
 */
void knight_rider()
{
  uint16_t v = 255;
  uint8_t l = 1;
  uint8_t s = 0;
  uint8_t d = 20;
  uint8_t k = 0;
  uint8_t w = 17;
  uint8_t c = 3;
  for( c = 0; c < 3; c++ ){
    for( k = 0; k < 18; k++){
      frame_buffer.startWrite();
      frame_buffer.writeFastHLine(0,k,18,v);
      frame_buffer.writeFastHLine(0,w-k,18,v);
      if( k > 0 ){
        frame_buffer.writeFastHLine(0,k-1,18,v/4);
        frame_buffer.writeFastHLine(0,w-k+1,18,v/4);
      }
      if( k > 1 ){
        frame_buffer.writeFastHLine(0,k-2,18,v/16);
        frame_buffer.writeFastHLine(0,w-k+2,18,v/16);
      }
      frame_buffer.execute();
    }
    delay(d);
  }
  for( c = 0; c < 3; c++ ){
    for( k = 0; k < 18; k++){
      frame_buffer.startWrite();
      frame_buffer.writeFastVLine(k,0,18,v);
      frame_buffer.writeFastVLine(w-k,0,18,v);
      if( k > 0 ){
        frame_buffer.writeFastVLine(k-1,0,18,v/4);
        frame_buffer.writeFastVLine(w-k+1,0,18,v/4);
      }
      if( k > 1 ){
        frame_buffer.writeFastVLine(k-2,0,18,v/16);
        frame_buffer.writeFastVLine(w-k+2,0,18,v/16);
      }
      frame_buffer.execute();
    }
    delay(d);
  }
  v = 128;
  for( c = 0; c < 3; c++ ){
    for( k = 0; k < 18; k++){
      frame_buffer.startWrite();
      frame_buffer.writeFastHLine(0,k,18,v);
      frame_buffer.writeFastHLine(0,w-k,18,v);
      frame_buffer.writeFastVLine(k,0,18,v);
      frame_buffer.writeFastVLine(w-k,0,18,v);
      if( k > 0 ){
        frame_buffer.writeFastHLine(0,k-1,18,v/4);
        frame_buffer.writeFastHLine(0,w-k+1,18,v/4);
        frame_buffer.writeFastVLine(k-1,0,18,v/4);
        frame_buffer.writeFastVLine(w-k+1,0,18,v/4);
      }
      frame_buffer.execute();
    }
    delay(d);
  }
}


/*
* Check for a or b keypress with debounce.
*/
bool ab_key(void)
{
  bool retVal = false;
  if( PIN_A_PRESS()    || PIN_B_PRESS() ){
        retVal = true;
        delay(100);
    }
  return retVal;
}


/*
 * Setup the badge. 
 */
void setup() {
  
  // initialize the frame buffer
  frame_buffer.init();
  frame_buffer.setAutoClear(true);
  frame_buffer.setTextSize(1);
  frame_buffer.setTextWrap(false);
  frame_buffer.setTextColor(255);
  frame_buffer.setRotation(3);

  // initialize our keys
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT);
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);

  // run the knight rider animation once.
  knight_rider();

  // Show the mr robot badge animation exactly once
  // unless the user presses the AB key. 
  while( !frame_buffer.animate_sidescroll(&ROBOT_scroll) ){
    frame_buffer.execute();
    delay(ROBOT_scroll.speed);
    if( ab_key() )
    {  
      delay(100);
      break;
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
