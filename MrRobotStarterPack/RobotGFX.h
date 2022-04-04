#ifndef ROBOT_GFX_H_
#define ROBOT_GFX_H_

#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3741.h>
#include <Fonts/FreeSerif9pt7b.h>

#define ARRAY_WIDTH 18 // LED Array Width
#define ARRAY_HEIGHT 18 // LED Array Height
// The I2C address of the ISSI chip interface. 
#define ADDR_GND 0x30
// Clock pins
#define SDA_PIN 4
#define SCL_PIN 5

TwoWire *i2c = &Wire;

/*
 * A simple pixel sprite kept in a flat array. 
 */
struct Bitmap
  {
    uint16_t w;
    uint16_t h;
    uint8_t* pixel_buffer;
    uint16_t x;
    uint16_t y;
    bool invert; 
    Bitmap(uint16_t w, uint16_t h, uint8_t* data, bool invert) : 
    w(w), h(h), pixel_buffer(data), invert(invert){
      x = 0;
      y = 0;
    }
  };

/*
 * A simple side scroll image kept in a flat array. X and Y
 * refer to the position being rendered in the image. 
 */
struct SideScroll
  {
    uint16_t w;
    uint16_t h;
    uint8_t* pixel_buffer;
    uint16_t x;
    uint16_t y;
    uint8_t orientation; // 0=horizontal 1=vertical
    bool invert; 
    uint16_t speed;
    SideScroll(uint16_t w, uint16_t h, uint8_t* data, uint8_t orientation, bool invert) :  w(w), h(h), pixel_buffer(data), orientation(orientation), invert(invert){
      x = 0;
      y = 0;
      speed = 20;
    }
  };

/*
 * A simple animation object where each frame of the animation is stored
 * sequentially as an image in the array.
 */

struct Animation
  {
    uint16_t w;
    uint16_t h;
    uint16_t total_w;
    uint16_t total_h;
    uint8_t* pixel_buffer;
    uint16_t total_frames;
    uint16_t current_frame;
    bool invert;
    uint16_t speed;
    Animation(uint16_t w, uint16_t h, uint8_t* data, uint16_t total_frames, uint16_t t_w, uint16_t t_h, bool invert) :
      w(w), h(h),pixel_buffer(data),total_frames(total_frames), invert(invert) {
        total_w = t_w;
        total_h = t_h;
        current_frame = 0;
        speed = 50;
      }
  };

/*
 * Mr. Robot Badge uses a non-standard layout relative to the ISSI 
 * chip. This lookup struct maps the (x,y) position of a given LED
 * to its device address.
 */

const uint16_t Lookup[ARRAY_HEIGHT][ARRAY_WIDTH] = {
  {  17,  47,  77, 107, 137, 167, 197, 227, 257,  18,  48,  78, 108, 138, 168, 198, 228, 258},
  {  16,  46,  76, 106, 136, 166, 196, 226, 256,  19,  49,  79, 109, 139, 169, 199, 229, 259},
  {  15,  45,  75, 105, 135, 165, 195, 225, 255,  20,  50,  80, 110, 140, 170, 200, 230, 260},
  {  14,  44,  74, 104, 134, 164, 194, 224, 254,  21,  51,  81, 111, 141, 171, 201, 231, 261},
  {  13,  43,  73, 103, 133, 163, 193, 223, 253,  22,  52,  82, 112, 142, 172, 202, 232, 262},
  {  12,  42,  72, 102, 132, 162, 192, 222, 252,  23,  53,  83, 113, 143, 173, 203, 233, 263},
  {  11,  41,  71, 101, 131, 161, 191, 221, 251,  24,  54,  84, 114, 144, 174, 204, 234, 264},
  {  10,  40,  70, 100, 130, 160, 190, 220, 250,  25,  55,  85, 115, 145, 175, 205, 235, 265},
  {   9,  39,  69,  99, 129, 159, 189, 219, 249,  26,  56,  86, 116, 146, 176, 206, 236, 266},
  {   8,  38,  68,  98, 128, 158, 188, 218, 248,  27,  57,  87, 117, 147, 177, 207, 237, 267},
  {   7,  37,  67,  97, 127, 157, 187, 217, 247,  28,  58,  88, 118, 148, 178, 208, 238, 268},
  {   6,  36,  66,  96, 126, 156, 186, 216, 246,  29,  59,  89, 119, 149, 179, 209, 239, 269},
  {   5,  35,  65,  95, 125, 155, 185, 215, 245, 270, 279, 288, 297, 306, 315, 324, 333, 342},
  {   4,  34,  64,  94, 124, 154, 184, 214, 244, 271, 280, 289, 298, 307, 316, 325, 334, 343},
  {   3,  33,  63,  93, 123, 153, 183, 213, 243, 272, 281, 290, 299, 308, 317, 326, 335, 344},
  {   2,  32,  62,  92, 122, 152, 182, 212, 242, 273, 282, 291, 300, 309, 318, 327, 336, 345},
  {   1,  31,  61,  91, 121, 151, 181, 211, 241, 274, 283, 292, 301, 310, 319, 328, 337, 346},
  {   0,  30,  60,  90, 120, 150, 180, 210, 240, 275, 284, 293, 302, 311, 320, 329, 338, 347}
};

/*
 * If we ran all of the LEDs at full brightness your retinas would burn out
 * and the badge mgiht catch fire. This array maps a brightness value to a PWM
 * value. 
 */
  const PROGMEM byte PWM_Gama64[64]= {
      0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
      0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
      0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
      0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
      0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
      0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
      0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
      0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff};


/*
 * This shim implements an interface to the Adafruit graphics 
 * interface and then extends it to support scrolls, animations, and bitmaps.
 * The adafruit text rendering is meant for larger screens so the scrolls are
 * a great way to print bulk text to the screen. 
 */
class AGFXShim : public Adafruit_GFX, public Adafruit_IS31FL3741_buffered
{
  public:
    uint8_t sda_pin = 4;
    uint8_t scl_pin = 5;
    bool autoClear = true;

    ////////////////////////////////////////////////////////////////////
    AGFXShim(uint8_t SDA_pin, uint8_t SCL_pin) : Adafruit_GFX((int16_t) ARRAY_WIDTH,(int16_t) ARRAY_HEIGHT){
      sda_pin = SDA_pin;
      scl_pin = SCL_pin;
    };
    ///////////////////////////////////////////////////////////////////
    void init(void){
      begin(ADDR_GND, i2c);
      i2c->setClock(1000000);
      setLEDscaling(0xFF);
      setGlobalCurrent(0x08);//global current
      enable(true);
    };
   ////////////////////////////////////////////////////////////////////
    virtual ~AGFXShim()
    {

    };

    ////////////////////////////////////////////////////////////////////
    /*
    * Clear out the frame buffer. 
    */
    void zero_buffer(){
      fill(0);
    };

    ////////////////////////////////////////////////////////////////////
    /*
     * Copy the current frame buffer to the ISSI chip using I2C and render
     * the results to the LED matrix. 
     */
    void execute() {
      show();
    }
    ////////////////////////////////////////////////////////////////////
    /*
    * Set a single pixel in the frame buffer. 
    */
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color){
      getBuffer()[Lookup[y][x]] = (uint8_t) color;
    };
    ////////////////////////////////////////////////////////////////////
    /*
     * If autoclear = true. Zero out the frame buffer once it is executed. 
     */
    void setAutoClear(bool setting){
      autoClear = setting;
    };
 
    ////////////////////////////////////////////////////////////////////
    /*
    * Animate a vertical side scroll. Once it has completely run once
    * return true otherwise return false. 
    */
    bool animate_sidescroll_v(SideScroll* ss)
    {
        bool retVal = false;
        uint16_t xidx,yidx,t;
        uint16_t ssidx;
        uint16_t y;
        uint8_t v;
        if( ss->y >= ss->h){
            ss->y = 0;
            retVal = true;
        }
        for( xidx = 0; xidx < ARRAY_WIDTH; xidx++ ){
          for( yidx = 0; yidx < ARRAY_HEIGHT; yidx++ ){
              // get the sidescroll value at the current xoff
            if( ss->y + yidx >= ss->h ){
              y = (ss->y+yidx)-ss->h;
              ssidx = xidx+(ss->w*y);
              v = (uint8_t) pgm_read_byte_near(ss->pixel_buffer+ssidx);
            }else{
              ssidx = xidx+(ss->w*(yidx+ss->y));
              v = (uint8_t) pgm_read_byte_near(ss->pixel_buffer+ssidx);
            }
            if( ss->invert ){
              v = 255 - v; 
            }
          writePixel(xidx,yidx,v);
          }
        }
      ss->y += 1;  
      return retVal;    
    }
    ////////////////////////////////////////////////////////////////////
    /*
    * Animate a horizontal side scroll. Once it has completely run once
    * return true otherwise return false. 
    */
    bool animate_sidescroll_h(SideScroll* ss)
    {
      bool retVal = false;
      uint16_t xidx,yidx,t;
      uint16_t ssidx;
      uint16_t x,y;
      uint8_t v;
      if( ss->x >= ss->w ){
        ss->x = 0;
        retVal = true;
      }
      for( xidx = 0; xidx < ARRAY_WIDTH; xidx++ ){
        for( yidx = 0; yidx < ARRAY_HEIGHT; yidx++ ){
          // get the sidescroll value at the current xoff
          if( ss->x + xidx >= ss->w ){
           // x = (ss->x+xidx)-ss->w;
           // ssidx = (x)+(ss->w*yidx);
           // v = (uint8_t) pgm_read_byte_near(ss->pixel_buffer+ssidx);
          }else{
            ssidx = (ss->x+xidx)+(ss->w*yidx);
            v = (uint8_t) pgm_read_byte_near(ss->pixel_buffer+ssidx);
          }

          if( ss->invert ){
            v = 255 - v; 
          }
          writePixel(xidx,yidx,v);
        }
      }
      ss->x += 1;  
      return retVal;  
    }
    ////////////////////////////////////////////////////////////////////
   /*
    * Animate a horz side scroll with a memory buffer that isn't in progmem.  
    * This is for the case where we want to store a nametag sidescroll in the 
    * spiff file system. 
    */    
    bool animate_sidescroll_h_local(SideScroll* ss)
    { // don't use progmem ... this is for user nametag stored in Spiff
      bool retVal = false;
      uint16_t xidx,yidx,t;
      uint16_t ssidx;
      uint16_t x,y;
      uint8_t v;
      if( ss->x >= ss->w){
        ss->x = 0;
        retVal = true;
      }
      for( xidx = 0; xidx < ARRAY_WIDTH; xidx++ ){
        for( yidx = 0; yidx < ARRAY_HEIGHT; yidx++ ){
          // get the sidescroll value at the current xoff
          if( ss->x + xidx >= ss->w ){
            //x = (ss->x+xidx)-ss->w;
            //ssidx = (x)+(ss->w*yidx);
            //v = (uint8_t) ss->pixel_buffer[ssidx];
          }else{
            ssidx = (ss->x+xidx)+(ss->w*yidx);
            v = (uint8_t) ss->pixel_buffer[ssidx];
          }

          if( ss->invert ){
            v = 255 - v; 
          }
          writePixel(xidx,yidx,v);
        }
      }
      ss->x += 1;  
      return retVal;  
    }    
   ////////////////////////////////////////////////////////////////////
   /*
   * Given a sidescroll copy it to the frame buffer. Return true
   * when we've cycled completely through the scroll. Execute 
   * must still be called after every call.
   */
   bool animate_sidescroll(SideScroll* ss){
      if( ss->orientation == 0 ){
        return animate_sidescroll_h(ss);
      }
      else{
        return animate_sidescroll_v(ss);
      }
    } 
  ////////////////////////////////////////////////////////////////////
  /*
   * Copy the current animation frame to the frame buffer. If the animation
   * has completely cycled through return true otherwise return false.
   */
  bool run_animation(Animation* a)
  {
    uint16_t xidx,yidx,t;
    uint16_t ssidx;
    uint8_t v;
    bool retVal = false;
    if( a->current_frame == a->total_frames-1)
    {
      retVal = true; // signal each cycle
    }
    if( a->current_frame >= a->total_frames){
      a->current_frame = 0;
    }
    for( xidx = 0; xidx < ARRAY_WIDTH; xidx++ ){
      for( yidx = 0; yidx < ARRAY_HEIGHT; yidx++ ){
          //      current frame x off        // y is over total array
          ssidx = ((a->current_frame*a->w)+xidx)+(a->total_w*yidx);
          v = (uint8_t) pgm_read_byte_near(a->pixel_buffer+ssidx);
          //v = a->pixel_buffer[ssidx];
          if( a->invert ){
            v = 255 - v;
          }
          writePixel(xidx,yidx,v);
      }
    }
    a->current_frame++;
    return retVal;
  }
  ////////////////////////////////////////////////////////////////////
  /*
   * Draw a bitmap at its specified position withs its given parameters. 
   * This is a lot clearner that the ada fruit version and acts like a sprite.
   * Unfortunately no partial rendering will be done if the bitmap is partially 
   * off screen.
   */
  void drawBitmap(Bitmap* b)
  {
    uint16_t xidx,yidx;
    uint16_t ssidx;
    uint16_t xloc,yloc;
    uint8_t v;
    if(b->x+b->w > ARRAY_WIDTH || 
       b->y+b->h > ARRAY_HEIGHT )
       {  // don't draw out of bounds stuff.
        return;
       }
    uint8_t tx,ty,bm_idx;
    for(yidx = 0; yidx < b->h; yidx++){
      for(xidx = 0; xidx < b->w; xidx++){
        ssidx = xidx + (yidx*b->w);
        v  = (uint8_t) pgm_read_byte_near(b->pixel_buffer+ssidx);
        if( b->invert ){
            v = 255 - v;
          }
        xloc = b->x+xidx;
        yloc = b->y+yidx;
        writePixel(xloc,yloc,v);
      }
    }
  }
  ////////////////////////////////////////////////////////////////////

}; // END CLASS

#endif
