/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1325 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

//#include <Wire.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1325.h"

#include "glcdfont.c"

// the memory buffer for the LCD

static uint8_t buffer[SSD1325_LCDHEIGHT * SSD1325_LCDWIDTH / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xC0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0C, 0x3E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF0,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xE0, 0xE0, 0xE0,
0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFD,
0xFB, 0x7B, 0xBF, 0xFF, 0xFF, 0xFC, 0x7F, 0xFF, 0xF7, 0xF7, 0xF7, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD,
0xFE, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0xFC, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xF1,
0xC1, 0x01, 0x00, 0x00, 0x70, 0x78, 0x7C, 0x7C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFC, 0xFC, 0xF8,
0xF0, 0x00, 0x00, 0xF0, 0xF8, 0xFC, 0xFC, 0x3C, 0x3C, 0x3C, 0x3C, 0x78, 0xFF, 0xFF, 0xFF, 0xFF,
0x00, 0x00, 0x70, 0x78, 0x7C, 0x7C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFC, 0xFC, 0xF8, 0xF0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x3D, 0x3D, 0x3D, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x70, 0x38,
0x3C, 0x3C, 0x3C, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC,
0xFC, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C,
0x00, 0x00, 0x00, 0x00, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x03,
0x03, 0x01, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF,
0xFF, 0x00, 0x00, 0x00, 0xFC, 0xFE, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xF7, 0xFF, 0xFF, 0xFF,
0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF,
0x00, 0x00, 0xFC, 0xFE, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xFF, 0xFF, 0xFF,
0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xF8, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF8, 0xF9, 0xF9, 0xF9,
0xF9, 0xF8, 0xF8, 0xF8, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF8, 0xF9, 0xF9, 0xF9, 0xF9,
0xF8, 0xF8, 0xF8, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF8, 0xF9, 0xF9, 0xF9, 0xF9, 0xF8,
0xF8, 0xF9, 0x09, 0xF9, 0x09, 0xD8, 0xB8, 0x78, 0x08, 0xF8, 0x09, 0xE9, 0xE9, 0x19, 0xF8, 0x08,
0xF8, 0xF8, 0x08, 0xF8, 0xD8, 0xA8, 0xA9, 0x69, 0xF9, 0xE9, 0x09, 0xE9, 0xF8, 0x09, 0xA9, 0x29,
0xD9, 0xF8, 0x08, 0xF9, 0x09, 0xA9, 0xA9, 0xF8, 0xD8, 0xA8, 0xA9, 0x69, 0xF9, 0xF9, 0xF9, 0xF9,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01,
0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01
};

// the most basic function, set a single pixel
void Adafruit_SSD1325::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

  // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
  case 3:
    swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }  

  // x is which column
  if (color == WHITE) 
    buffer[x+ (y/8)*SSD1325_LCDWIDTH] |= _BV((y%8));  
  else
    buffer[x+ (y/8)*SSD1325_LCDWIDTH] &= ~_BV((y%8)); 
}

void Adafruit_SSD1325::begin(void) {
  // set pin directions
  pinMode(sid, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(dc, OUTPUT);
  pinMode(rst, OUTPUT);
  pinMode(cs, OUTPUT);

  clkport     = portOutputRegister(digitalPinToPort(sclk));
  clkpinmask  = digitalPinToBitMask(sclk);
  mosiport    = portOutputRegister(digitalPinToPort(sid));
  mosipinmask = digitalPinToBitMask(sid);
  csport    = portOutputRegister(digitalPinToPort(cs));
  cspinmask = digitalPinToBitMask(cs);
  dcport    = portOutputRegister(digitalPinToPort(dc));
  dcpinmask = digitalPinToBitMask(dc);

  digitalWrite(rst, HIGH);
  // VDD (3.3V) goes high at start, lets just chill for a ms
  delay(1);
  // bring reset low
  digitalWrite(rst, LOW);
  // wait 10ms
  delay(10);
  // bring out of reset
  digitalWrite(rst, HIGH);

  command(SSD1325_DISPLAYOFF); /* display off */
  command(SSD1325_SETCLOCK); /* set osc division */
  command(0xF1); /* 145 */
  command(SSD1325_SETMULTIPLEX ); /* multiplex ratio */
  command(0x3f); /* duty = 1/64 */
  command( SSD1325_SETOFFSET); /* set display offset --- */
  command(0x4C); /* 76 */
  command(SSD1325_SETSTARTLINE); /*set start line */
  command(0x00); /* ------ */
  command(SSD1325_MASTERCONFIG); /*Set Master Config DC/DC Converter*/
  command(0x02);
  command(SSD1325_SETREMAP); /* set segment remap------ */
  command(0x56);
  //command(0x52);
  command(SSD1325_SETCURRENT + 0x2); /* Set Full Current Range */
  command(SSD1325_SETGRAYTABLE);
  command(0x01);
  command(0x11);
  command(0x22);
  command(0x32);
  command(0x43);
  command(0x54);
  command(0x65);
  command(0x76);

  command(SSD1325_SETCONTRAST); /* set contrast current */
  command(0x7F);  // max!
  
  command(SSD1325_SETROWPERIOD);
  command(0x51);
  command(SSD1325_SETPHASELEN);
  command(0x55);
  command(SSD1325_SETPRECHARGECOMP);
  command(0x02);
  command(SSD1325_SETPRECHARGECOMPENABLE);
  command(0x28);
  command(SSD1325_SETVCOMLEVEL); // Set High Voltage Level of COM Pin
  command(0x1C); //?
  command(SSD1325_SETVSL); // set Low Voltage Level of SEG Pin 
  command(0x0D|0x02);
  
  command(SSD1325_NORMALDISPLAY); /* set display mode */
  /* Clear Screen */  
 // command(0x23); /*set graphic acceleration commmand */
 // command(SSD1325_GFXACCEL);
 // command(SSD1325_DRAWRECT); /* draw rectangle */
 // command(0x00); /* Gray Scale Level 1 */
 // command(0x00); /* Gray Scale Level 3 & 2 */
 // command(0x3f); /* Gray Scale Level 3 & 2 */
 // command(0x3f); /* Gray Scale Level 3 & 2 */
 // command(0x00); /* Gray Scale Level 3 & 2 */

  command(SSD1325_DISPLAYON); /* display ON */
}

void Adafruit_SSD1325::invertDisplay(uint8_t i) {
  if (i) {
    command(SSD1325_INVERTDISPLAY);
  } else {
    command(SSD1325_NORMALDISPLAY);
  }
}

void Adafruit_SSD1325::command(uint8_t c) { 
  //digitalWrite(cs, HIGH);
  *csport |= cspinmask;
  //digitalWrite(dc, LOW);
  *dcport &= ~dcpinmask;
  //digitalWrite(cs, LOW);
  *csport &= ~cspinmask;
  fastSPIwrite(c);
  //digitalWrite(cs, HIGH);
  *csport |= cspinmask;
}

void Adafruit_SSD1325::data(uint8_t c) {
  //digitalWrite(cs, HIGH);
  *csport |= cspinmask;
  //digitalWrite(dc, HIGH);
  *dcport |= dcpinmask;
  //digitalWrite(cs, LOW);
  *csport &= ~cspinmask;
  fastSPIwrite(c);
  //digitalWrite(cs, HIGH);
  *csport |= cspinmask;
}

void Adafruit_SSD1325::display(void) {
  
  command(0x15); /* set column address */
  command(0x00); /* set column start address */
  command(0x3f); /* set column end address */
  command(0x75); /* set row address */
  command(0x00); /* set row start address */
  command(0x3f); /* set row end address */

  *csport |= cspinmask;
  *dcport |= dcpinmask;
  *csport &= ~cspinmask;

  for (uint16_t x=0; x<128; x+=2) {
    for (uint16_t y=0; y<64; y+=8) { // we write 8 pixels at once
      uint8_t left8 = buffer[y*16+x];
      uint8_t right8 = buffer[y*16+x+1];
      for (uint8_t p=0; p<8; p++) {
	uint8_t d = 0;
	if (left8 & (1 << p)) d |= 0xF0;
	if (right8 & (1 << p)) d |= 0x0F;
	fastSPIwrite(d);
      }
    }
  }
  *csport |= cspinmask;
}

// clear everything
void Adafruit_SSD1325::clearDisplay(void) {
  memset(buffer, 0, (SSD1325_LCDWIDTH*SSD1325_LCDHEIGHT/8));
}


inline void Adafruit_SSD1325::fastSPIwrite(uint8_t d) {
  
  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    *clkport &= ~clkpinmask;
    if(d & bit) *mosiport |=  mosipinmask;
    else        *mosiport &= ~mosipinmask;
    *clkport |=  clkpinmask;
  }
  //*csport |= cspinmask;
}

inline void Adafruit_SSD1325::slowSPIwrite(uint8_t d) {
 for (int8_t i=7; i>=0; i--) {
   digitalWrite(sclk, LOW);
   if (d & _BV(i)) {
     digitalWrite(sid, HIGH);
   } else {
     digitalWrite(sid, LOW);
   }
   digitalWrite(sclk, HIGH);
 }
}

