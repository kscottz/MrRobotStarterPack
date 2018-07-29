#ifndef ROBOT_GFX_H_
#define ROBOT_GFX_H_

#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSerif9pt7b.h>
#include "brzo_i2c.h"


#define ARRAY_WIDTH 18 // LED Array Width
#define ARRAY_HEIGHT 18 // LED Array Height
#define BULK_CMD_SZ 4 // Size of a single LED command
// The ISSI chip has two pages of differing size. 
#define PAGE_0_SZ 180
#define PAGE_1_SZ 171
// The I2C address of the ISSI chip interface. 
#define ADDR_GND 0x60
// Clock pins
#define SDA_PIN 4
#define SCL_PIN 5


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
struct Lookup
  {
    uint8_t x; // x pixel, if the address is unused then set to -1
    uint8_t y; // y pixel, if the address is unused set to -1
    uint8_t addr;// the addr !!! THIS IS ONLY FOR REFERENCE / DEBUG
    Lookup(uint8_t x, uint8_t y, uint8_t addr) : x(x), y(y), addr(addr){}
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
class AGFXShim : public Adafruit_GFX
{
  public:
    // This is where we keep the next set of data to render. 
    uint16_t frame_buffer[18][18];
    uint16_t fame_buffer_sz = 324;

    // Fill this out *SEQUENTIALLY* from Page0 address 00 to A7.
    // If an LED's slot is unused set x and y to zero
    // The addr field is not used, but kept for reference.

    /*
     * The following look up tables are used to map LED X,Y positions to their respective page and
     * and address on the ISSI chips. The mapping is a bit strange due to the routing. The idea here 
     * is that for any given address we can quickly look up its value in the frame buffer and squirt 
     * that over to ISSI LED driver. 
     */
    int16_t lut0_sz = PAGE_0_SZ;
    Lookup const page0LUT[PAGE_0_SZ] = { Lookup(0,17,0x00),
      Lookup(0,16,0x01),  Lookup(0,15,0x02),  Lookup(0,14,0x03),  Lookup(0,13,0x04),  Lookup(0,12,0x05),
      Lookup(0,11,0x06),  Lookup(0,10,0x07),  Lookup(0,9,0x08),   Lookup(0,8,0x09),   Lookup(0,7,0x0A),
      Lookup(0,6,0x0B),   Lookup(0,5,0x0C ),  Lookup(0,4,0x0D),   Lookup(0,3,0x0E),   Lookup(0,2,0x0F),
      Lookup(0,1,0x10),   Lookup(0,0,0x11),   Lookup(9,0,0x12),   Lookup(9,1,0x13),   Lookup(9,2,0x14),
      Lookup(9,3,0x15),   Lookup(9,4,0x16),   Lookup(9,5,0x17),   Lookup(9,6,0x18),   Lookup(9,7,0x19),
      Lookup(9,8,0x1A),   Lookup(9,9,0x1B),   Lookup(9,10,0x1C),  Lookup(9,11,0x1D),  Lookup(1,17,0x1E),
      Lookup(1,16,0x1F),  Lookup(1,15,0x20),  Lookup(1,14,0x21),  Lookup(1,13,0x22),  Lookup(1,12,0x23),
      Lookup(1,11,0x24),  Lookup(1,10,0x25),  Lookup(1,9,0x26),   Lookup(1,8,0x27),   Lookup(1,7,0x28),
      Lookup(1,6,0x29),   Lookup(1,5,0x2A),   Lookup(1,4,0x2B),   Lookup(1,3,0x2C),   Lookup(1,2,0x2D),
      Lookup(1,1,0x2E),   Lookup(1,0,0x2F),   Lookup(10,0,0x30),  Lookup(10,1,0x31),  Lookup(10,2,0x32),
      Lookup(10,3,0x33),  Lookup(10,4,0x34),  Lookup(10,5,0x35),  Lookup(10,6,0x36),  Lookup(10,7,0x37),
      Lookup(10,8,0x38),  Lookup(10,9,0x39),  Lookup(10,10,0x3A), Lookup(10,11,0x3B), Lookup(2,17,0x3C),
      Lookup(2,16,0x3D),  Lookup(2,15,0x3E),  Lookup(2,14,0x3F),  Lookup(2,13,0x40),  Lookup(2,12,0x41),
      Lookup(2,11,0x42),  Lookup(2,10,0x43),  Lookup(2,9,0x44),   Lookup(2,8,0x45),   Lookup(2,7,0x46),
      Lookup(2,6,0x47),   Lookup(2,5,0x48),   Lookup(2,4,0x49),   Lookup(2,3,0x4A),   Lookup(2,2,0x4B),
      Lookup(2,1,0x4C),   Lookup(2,0,0x4D),   Lookup(11,0,0x4E),  Lookup(11,1,0x4F),  Lookup(11,2,0x50),
      Lookup(11,3,0x51),  Lookup(11,4,0x52),  Lookup(11,5,0x53),  Lookup(11,6,0x54),  Lookup(11,7,0x55),
      Lookup(11,8,0x56),  Lookup(11,9,0x57),  Lookup(11,10,0x58), Lookup(11,11,0x59), Lookup(3,17,0x5A),
      Lookup(3,16,0x5B),  Lookup(3,15,0x5C),  Lookup(3,14,0x5D),  Lookup(3,13,0x5E),  Lookup(3,12,0x5F),
      Lookup(3,11,0x60),  Lookup(3,10,0x61),  Lookup(3,9,0x62),   Lookup(3,8,0x63),   Lookup(3,7,0x64),
      Lookup(3,6,0x65),   Lookup(3,5,0x66),   Lookup(3,4,0x67),   Lookup(3,3,0x68),   Lookup(3,2,0x69),
      Lookup(3,1,0x6A),   Lookup(3,0,0x6B),   Lookup(12,0,0x6C),  Lookup(12,1,0x6D),  Lookup(12,2,0x6E),
      Lookup(12,3,0x6F),  Lookup(12,4,0x70),  Lookup(12,5,0x71),  Lookup(12,6,0x72),  Lookup(12,7,0x73),
      Lookup(12,8,0x74),  Lookup(12,9,0x75),  Lookup(12,10,0x76), Lookup(12,11,0x77), Lookup(4,17,0x78),
      Lookup(4,16,0x79),  Lookup(4,15,0x7A),  Lookup(4,14,0x7B ), Lookup(4,13,0x7C),  Lookup(4,12,0x7D),
      Lookup(4,11,0x7E ), Lookup(4,10,0x7F),  Lookup(4,9,0x80),   Lookup(4,8,0x81),   Lookup(4,7,0x82),
      Lookup(4,6,0x83),   Lookup(4,5,0x84),   Lookup(4,4,0x85),   Lookup(4,3,0x86),   Lookup(4,2,0x87),
      Lookup(4,1,0x88),   Lookup(4,0,0x89),   Lookup(13,0,0x8A),  Lookup(13,1,0x8B),  Lookup(13,2,0x8C),
      Lookup(13,3,0x8D),  Lookup(13,4,0x8E),  Lookup(13,5,0x8F),  Lookup(13,6,0x90),  Lookup(13,7,0x91),
      Lookup(13,8,0x92),  Lookup(13,9,0x93),  Lookup(13,10,0x94), Lookup(13,11,0x95), Lookup(5,17,0x96),
      Lookup(5,16,0x97),  Lookup(5,15,0x98),  Lookup(5,14,0x99),  Lookup(5,13,0x9A),  Lookup(5,12,0x9B),
      Lookup(5,11,0x9C),  Lookup(5,10,0x9D),  Lookup(5,9,0x9E),   Lookup(5,8,0x9F),   Lookup(5,7,0xA0),
      Lookup(5,6,0xA1),   Lookup(5,5,0xA2),   Lookup(5,4,0xA3),   Lookup(5,3,0xA4),   Lookup(5,2,0xA5),
      Lookup(5,1,0xA6),   Lookup(5,0,0xA7),   Lookup(14,0,0xA8),  Lookup(14,1,0xA9),  Lookup(14,2,0xAA),
      Lookup(14,3,0xAB),  Lookup(14,4,0xAC),  Lookup(14,5,0xAD),  Lookup(14,6,0xAE),  Lookup(14,7,0xAF),
      Lookup(14,8,0xB0),  Lookup(14,9,0xB1),  Lookup(14,10,0xB2), Lookup(14,11,0xB3)};

    int16_t lut1_sz = PAGE_1_SZ;
    Lookup const page1LUT[PAGE_1_SZ] = {  Lookup(6,17,0x00),
      Lookup(6,16,0x01),  Lookup(6,15,0x02),  Lookup(6,14,0x03),  Lookup(6,13,0x04),  Lookup(6,12,0x05),
      Lookup(6,11,0x06),  Lookup(6,10,0x07),  Lookup(6,9,0x08),   Lookup(6,8,0x09),   Lookup(6,7,0x0A),
      Lookup(6,6,0x0B),   Lookup(6,5,0x0C),   Lookup(6,4,0x0D),   Lookup(6,3,0x0E),   Lookup(6,2,0x0F),
      Lookup(6,1,0x10),   Lookup(6,0,0x11),   Lookup(15,0,0x12),  Lookup(15,1,0x13),  Lookup(15,2,0x14),
      Lookup(15,3,0x15),  Lookup(15,4,0x16),  Lookup(15,5,0x17),  Lookup(15,6,0x18),  Lookup(15,7,0x19),
      Lookup(15,8,0x1A),  Lookup(15,9,0x1B),  Lookup(15,10,0x1C), Lookup(15,11,0x1D), Lookup(7,17,0x1E),
      Lookup(7,16,0x1F),  Lookup(7,15,0x20),  Lookup(7,14,0x21),  Lookup(7,13,0x22),  Lookup(7,12,0x23),
      Lookup(7,11,0x24),  Lookup(7,10,0x25),  Lookup(7,9,0x26),   Lookup(7,8,0x27),   Lookup(7,7,0x28),
      Lookup(7,6,0x29),   Lookup(7,5,0x2A),   Lookup(7,4,0x2B),   Lookup(7,3,0x2C),   Lookup(7,2,0x2D),
      Lookup(7,1,0x2E),   Lookup(7,0,0x2F),   Lookup(16,0,0x30),  Lookup(16,1,0x31),  Lookup(16,2,0x32),
      Lookup(16,3,0x33),  Lookup(16,4,0x34),  Lookup(16,5,0x35),  Lookup(16,6,0x36),  Lookup(16,7,0x37),
      Lookup(16,8,0x38),  Lookup(16,9,0x39),  Lookup(16,10,0x3A), Lookup(16,11,0x3B), Lookup(8,17,0x3C),
      Lookup(8,16,0x3D),  Lookup(8,15,0x3E),  Lookup(8,14,0x3F),  Lookup(8,13,0x40),  Lookup(8,12,0x41),
      Lookup(8,11,0x42),  Lookup(8,10,0x43),  Lookup(8,9,0x44),   Lookup(8,8,0x45),   Lookup(8,7,0x46),
      Lookup(8,6,0x47),   Lookup(8,5,0x48),   Lookup(8,4,0x49),   Lookup(8,3,0x4A),   Lookup(8,2,0x4B),
      Lookup(8,1,0x4C),   Lookup(8,0,0x4D),   Lookup(17,0,0x4E),  Lookup(17,1,0x4F),  Lookup(17,2,0x50),
      Lookup(17,3,0x51),  Lookup(17,4,0x52),  Lookup(17,5,0x53),  Lookup(17,6,0x54),  Lookup(17,7,0x55),
      Lookup(17,8,0x56),  Lookup(17,9,0x57),  Lookup(17,10,0x58), Lookup(17,11,0x59), Lookup(9,12,0x5A),
      Lookup(9,13,0x5B),  Lookup(9,14,0x5C),  Lookup(9,15,0x5D),  Lookup(9,16,0x5E),  Lookup(9,17,0x5F),
      Lookup(-1,-1,0x60), Lookup(-1,-1,0x61), Lookup(-1,-1,0x62), Lookup(10,12,0x63), Lookup(10,13,0x64),
      Lookup(10,14,0x65), Lookup(10,15,0x66), Lookup(10,16,0x67), Lookup(10,17,0x68), Lookup(-1,-1,0x69),
      Lookup(-1,-1,0x6A), Lookup(-1,-1,0x6B), Lookup(11,12,0x6C), Lookup(11,13,0x6D), Lookup(11,14,0x6E),
      Lookup(11,15,0x6F), Lookup(11,16,0x70), Lookup(11,17,0x71), Lookup(-1,-1,0x72), Lookup(-1,-1,0x73),
      Lookup(-1,-1,0x74), Lookup(12,12,0x75), Lookup(12,13,0x76), Lookup(12,14,0x77), Lookup(12,15,0x78),
      Lookup(12,16,0x79), Lookup(12,17,0x7A), Lookup(-1,-1,0x7B), Lookup(-1,-1,0x7C), Lookup(-1,-1,0x7D),
      Lookup(13,12,0x7E), Lookup(13,13,0x7F), Lookup(13,14,0x80), Lookup(13,15,0x81), Lookup(13,16,0x82),
      Lookup(13,17,0x83), Lookup(-1,-1,0x84), Lookup(-1,-1,0x85), Lookup(-1,-1,0x86), Lookup(14,12,0x87),
      Lookup(14,13,0x88), Lookup(14,14,0x89), Lookup(14,15,0x8A), Lookup(14,16,0x8B), Lookup(14,17,0x8C),
      Lookup(-1,-1,0x8D), Lookup(-1,-1,0x8E), Lookup(-1,-1,0x8F), Lookup(15,12,0x90), Lookup(15,13,0x91),
      Lookup(15,14,0x92), Lookup(15,15,0x93), Lookup(15,16,0x94), Lookup(15,17,0x95), Lookup(-1,-1,0x96),
      Lookup(-1,-1,0x97), Lookup(-1,-1,0x98), Lookup(16,12,0x99), Lookup(16,13,0x9A), Lookup(16,14,0x9B),
      Lookup(16,15,0x9C), Lookup(16,16,0x9D), Lookup(16,17,0x9E), Lookup(-1,-1,0x9F), Lookup(-1,-1,0xA0),
      Lookup(-1,-1,0xA1), Lookup(17,12,0xA2), Lookup(17,13,0xA3), Lookup(17,14,0xA4), Lookup(17,15,0xA5),
      Lookup(17,16,0xA6), Lookup(17,17,0xA7), Lookup(-1,-1,0xA8), Lookup(-1,-1,0xA9), Lookup(-1,-1,0xAA)};

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
      uint8_t data = 255;
      uint8_t i = 0;
      // Use I2C to setup ISSI chip
      brzo_i2c_setup(sda_pin, scl_pin, 2000);
      writeByte(ADDR_GND,0xfe,0xc5);//unlock
      writeByte(ADDR_GND,0xfD,0x02);//write page 2
      for(i=0;i<0xB4;i++){
        writeByte(ADDR_GND,i,data);//R LED Scaling
      }

      writeByte(ADDR_GND,0xfe,0xc5);//unlock
      writeByte(ADDR_GND,0xfD,0x03);//write page 3
      for(i=0;i<0xAB;i++){
        writeByte(ADDR_GND,i,data);//R LED Scaling
      }

      writeByte(ADDR_GND,0xfe,0xc5);//unlock
      writeByte(ADDR_GND,0xfD,0x00);//write page 0
      for(i=0;i<0xB4;i++){
        writeByte(ADDR_GND,i,0x00);//write all PWM set 0x00
      }
      writeByte(ADDR_GND,0xfe,0xc5);//unlock
      writeByte(ADDR_GND,0xfD,0x01);//write page 1

      for(i=0;i<0xAB;i++){
        writeByte(ADDR_GND,i,0x00);//write all PWM set 0x00
      } //init all the PWM data to 0

      writeByte(ADDR_GND,0xfe,0xc5);//unlock
      writeByte(ADDR_GND,0xfD,0x04);//write page 4
      writeByte(ADDR_GND,0x01,0x08);//global current
      writeByte(ADDR_GND,0x00,0x01);//normal operation
    };
    ////////////////////////////////////////////////////////////////////
    /* Write a single byte to the ISSI chip using I2C
    */
    void writeByte(uint8_t Dev_Add,uint8_t Reg_Add,uint8_t Reg_Dat){
      uint8_t buffer[2];
      brzo_i2c_start_transaction(Dev_Add/2, 400); // open I2C to ISSI at 400kHz
      buffer[0] = Reg_Add;                    // select register address
      buffer[1] = Reg_Dat;                    // select register data
      brzo_i2c_write(buffer, 2, false);       // write two bites with no repeated start
      brzo_i2c_end_transaction();
    }
   ////////////////////////////////////////////////////////////////////
    virtual ~AGFXShim()
    {

    };
    ////////////////////////////////////////////////////////////////////
    /*
    * Clear out the frame buffer. 
    */
    void zero_buffer(){
      uint8_t i,j;
      for(i = 0; i < ARRAY_WIDTH; i++){
        for(j = 0; j < ARRAY_HEIGHT; j++){
          frame_buffer[i][j]=0;
        }
      }
    };

    ////////////////////////////////////////////////////////////////////
    /*
     * Copy the current frame buffer to the ISSI chip using I2C and render
     * the results to the LED matrix. 
     */
    void execute() {
      uint16_t idx = 0;
      uint8_t buffer[2];
      uint8_t buffSz = 2;

      uint8_t buff0[lut0_sz+BULK_CMD_SZ];

      // Copy the frame buff into a local buffer that's going to get sent to
      // the chip. The chip layout is weird so we use a LUT to get it right
      for( idx = 0; idx < lut0_sz; idx++ ){ // we're not using the full address space of the driver chip
           if( page0LUT[idx].x >= 0 ){
             buff0[BULK_CMD_SZ+idx]= frame_buffer[page0LUT[idx].x][page0LUT[idx].y];
           }
     }
     // Same for page two of the chip
     uint8_t buff1[lut1_sz+BULK_CMD_SZ];
     for( idx = 0; idx < lut1_sz; idx++ ){
        if( page1LUT[idx].x >= 0 ){
             buff1[BULK_CMD_SZ+idx]= frame_buffer[page1LUT[idx].x][page1LUT[idx].y];
           }
      }
      brzo_i2c_start_transaction(0x60/2, 400); // open I2C to ISSI at 400kHz
      buffer[0] = 0xFE; // unlock
      buffer[1] = 0xC5; // unlock
      brzo_i2c_write(buffer, 2, false); // write two bites with no repeated start
      brzo_i2c_end_transaction();

      brzo_i2c_start_transaction(0x60/2, 400); // open I2C to ISSI at 400kHz
      buff0[0] = 0xFD; // Set page
      buff0[1] = 0x00; // page 0
      buff0[2] = 0x00; // first index
      brzo_i2c_write(buff0, lut0_sz+BULK_CMD_SZ, false); // Dump the page 0 buffer
      brzo_i2c_end_transaction();

      // now repeat for page 1
      brzo_i2c_start_transaction(0x60/2, 400); // open I2C to ISSI at 400kHz
      buffer[0] = 0xFE; // unlock
      buffer[1] = 0xC5; // unlock
      brzo_i2c_write(buffer, 2, false);       // write two bites with no repeated start
      brzo_i2c_end_transaction();

      brzo_i2c_start_transaction(0x60/2, 400); // open I2C to ISSI at 400kHz
      buff1[0] = 0xFD; // Set page
      buff1[1] = 0x01; // page 1
      buff1[2] = 0x00; // first index
      brzo_i2c_write(buff1, lut1_sz+BULK_CMD_SZ, false);//lut0_sz, false);       // write two bites with no repeated start
      brzo_i2c_end_transaction();
      if( autoClear ){
        zero_buffer();
      }
    };
    ////////////////////////////////////////////////////////////////////
    /*
    * Set a single pixel in the frame buffer. 
    */
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color){
      frame_buffer[x][y] = (uint8_t) color;
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
