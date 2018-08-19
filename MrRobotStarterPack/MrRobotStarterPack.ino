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

  // set the cube animation to run inverted
  CUBE_animation.invert = true;

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

/*
 * Write a random frame to the frame buffer 
 * and execute it.
 */
void bubbles()
{
  uint16_t x,y;
  uint16_t c,r,i,j;
  for( i = 0; i < 5; i++ ){
    for( j = 0; j < 324; j++){
      x = random(0,18);
      y = random(0,18);
      c = random(0,255);
      frame_buffer.writePixel(x,y,c);
    } 
    frame_buffer.execute();
    delay(20);
  }
}

#define BOARD_WIDTH 18
#define BOARD_HEIGHT 18

int xadd(int i, int a) {
  i += a;
  while (i < 0) i += BOARD_WIDTH;
  while (i >= BOARD_WIDTH) i -= BOARD_WIDTH;
  return i;
}

int yadd(int i, int a) {
  i += a;
  while (i < 0) i += BOARD_HEIGHT;
  while (i >= BOARD_HEIGHT) i -= BOARD_HEIGHT;
  return i;
}

int adjacent_to(int board[][BOARD_HEIGHT], int i, int j) {
  int k, l, count;

  count = 0;
  for (k=-1; k<=1; k++) for (l=-1; l<=1; l++)
    if (k || l)
      if (board[xadd(i,k)][yadd(j,l)]) count++;
  return count;
}

void play(int board[][BOARD_HEIGHT])
{
  int i, j, a, newboard[BOARD_WIDTH][BOARD_HEIGHT];

  for (i=0; i<BOARD_WIDTH; i++) {
    for (j=0; j<BOARD_HEIGHT; j++) {
      a = adjacent_to(board, i, j);
      if (a == 2) newboard[i][j] = board[i][j];
      if (a == 3) newboard[i][j] = 1;
      if (a < 2) newboard[i][j] = 0;
      if (a > 3) newboard[i][j] = 0;
    }
  }

  for (i=0; i<BOARD_WIDTH; i++) {
    for (j=0; j<BOARD_HEIGHT; j++) {
      board[i][j] = newboard[i][j];
    }
  }
}

void initialize_board(int board[][BOARD_HEIGHT]) {
  int i, j;

  for (i=0; i<BOARD_WIDTH; i++) 
    for (j=0; j<BOARD_HEIGHT; j++) 
      board[i][j] = random(0,2);
}

void life()
{
  int board[BOARD_WIDTH][BOARD_HEIGHT], i, j;
  initialize_board(board);

  while(!PIN_B_PRESS())
  {
    for (i=0; i<BOARD_WIDTH; i++) 
    { 
      for (j=0; j<BOARD_HEIGHT; j++) 
      {
        frame_buffer.writePixel(i,j,board[i][j]?255:0);
      } 
    }
    frame_buffer.execute();
    delay(20);
    play(board);
    if( PIN_RIGHT_PRESS() ) initialize_board(board);
  }
}

void clear_board(int board[][BOARD_HEIGHT]) {
  int i, j;

  for (i=0; i<BOARD_WIDTH; i++) 
    for (j=0; j<BOARD_HEIGHT; j++) 
      board[i][j] = 0;
}

void fire()
{
  int board[BOARD_WIDTH][BOARD_HEIGHT], i, j;
  clear_board(board);

  while(!PIN_B_PRESS())
  {
    for (i=0; i<BOARD_WIDTH; i++) 
        board[i][BOARD_HEIGHT-1] = random(0,255);

    for (j=0; j<BOARD_HEIGHT-1; j++)
    {
      for (i=1; i<BOARD_WIDTH-1; i++)
      {
        board[i][j] = ((board[i-1][j+1] + board[i][j+1] + board[i+1][j+1])/3)-10;
	if (board[i][j]<0) board[i][j] = 0;
      }   
    }

    for (i=0; i<BOARD_WIDTH; i++) 
    { 
      for (j=0; j<BOARD_HEIGHT; j++) 
      {
        frame_buffer.writePixel(i,j,board[i][j]);
      } 
    }
    frame_buffer.execute();
    delay(20);
    if( PIN_LEFT_PRESS() ) clear_board(board);
  }
}

void loop() {
  uint16_t i = 0;
  // put your main code here, to run repeatedly:
  if( PIN_A_PRESS() ){ // if you press A key
    // while the animation is not done write it to the frame buffer
    while(!frame_buffer.animate_sidescroll(&INVADERS_scroll)){
      frame_buffer.execute(); // display it
      delay(INVADERS_scroll.speed); // wait to render the next frame. 
    }
  }
  else if( PIN_B_PRESS() ){
    while(!frame_buffer.animate_sidescroll(&SUPREME_scroll)){
      frame_buffer.execute();
      delay(SUPREME_scroll.speed);
    }  
  }
  else if( PIN_UP_PRESS() ){
    while(!frame_buffer.run_animation(&CUBE_animation)){
      frame_buffer.execute();
      delay(CUBE_animation.speed);
    } 
  }
  else if( PIN_DOWN_PRESS() ){
    while(!frame_buffer.run_animation(&SKULLIMATION_animation)){
      frame_buffer.execute();
      delay(SKULLIMATION_animation.speed);
    }   
  }
  else if( PIN_LEFT_PRESS() ){
    fire();
    for( i = 0; i < 5; i++ ){
      frame_buffer.drawBitmap(&FROG_bitmap);
      frame_buffer.execute();
      delay(200);
    }
  }
  else if( PIN_RIGHT_PRESS() ){
    life();
    for( i = 0; i < 5; i++ ){
      frame_buffer.drawBitmap(&SKULL_bitmap);
      frame_buffer.execute();
      delay(200);  
    }
  }
  else{
    // if no keys are pressed run the random animation
    bubbles();
  }
  

}
