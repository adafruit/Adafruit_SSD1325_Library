/*!
 * @file Adafruit_SSD1325.h
 */
#ifndef _Adafruit_SSD1325_H_
#define _Adafruit_SSD1325_H_

#include "Arduino.h"

#ifdef __arm__
#define _BV(b) (1 << (b))
#endif

#include <Adafruit_GFX.h>

#define BLACK 0 //!< Draw 'off' pixels
#define WHITE 1 //!< Draw 'on' pixels

/*=========================================================================
    SSD1325 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1325_128_64  128x64 pixel display

    SSD1325_128_32  128x32 pixel display

    You also need to set the LCDWIDTH and LCDHEIGHT defines to an
    appropriate size

    -----------------------------------------------------------------------*/
#define SSD1325_128_64 //!< Way to specify 128x32 screen
/*=========================================================================*/

#if defined SSD1325_128_64 && defined SSD1325_128_32
#error "Only one SSD1325 display can be specified at once in SSD1325.h"
#endif
#if !defined SSD1325_128_64 && !defined SSD1325_128_32
#error "At least one SSD1325 display must be specified in SSD1325.h"
#endif

#if defined SSD1325_128_64
#define SSD1325_LCDWIDTH 128 //!< Width w/SSD1325_128_64 defined
#define SSD1325_LCDHEIGHT 64 //!< Height w/SSD1325_128_64 defined
#endif
#if defined SSD1325_128_32
#define SSD1325_LCDWIDTH 128 //!< Width w/SSD1325_128_32 defined
#define SSD1325_LCDHEIGHT 32 //!< Height w/SSD1325_128_32 defined
#endif

#define SSD1325_SETCOLADDR 0x15  //!< Set column address
#define SSD1325_SETROWADDR 0x75  //!< Set row address
#define SSD1325_SETCONTRAST 0x81 //!< Set contrast current
#define SSD1325_SETCURRENT 0x84  //!< Set current range

#define SSD1325_SETREMAP 0xA0      //!< Set re-map
#define SSD1325_SETSTARTLINE 0xA1  //!< Set display start line
#define SSD1325_SETOFFSET 0xA2     //!< Set display offset
#define SSD1325_NORMALDISPLAY 0xA4 //!< Set display mode normal
#define SSD1325_DISPLAYALLON 0xA5  //!< Set entire display on
#define SSD1325_DISPLAYALLOFF 0xA6 //!< Set entire display off
#define SSD1325_INVERTDISPLAY 0xA7 //!< Invert display
#define SSD1325_SETMULTIPLEX 0xA8  //!< Set multiplex ratio
#define SSD1325_MASTERCONFIG 0xAD  //!< Set master configuration
#define SSD1325_DISPLAYOFF 0xAE    //!< Set display off
#define SSD1325_DISPLAYON 0xAF     //!< Set display on

#define SSD1325_SETPRECHARGECOMPENABLE                                         \
  0xB0                                   //!< Set pre-charge compensation enable
#define SSD1325_SETPHASELEN 0xB1         //!< Set phase length
#define SSD1325_SETROWPERIOD 0xB2        //!< Set row period
#define SSD1325_SETCLOCK 0xB3            //!< Set display clock divide ratio
#define SSD1325_SETPRECHARGECOMP 0xB4    //!< Set pre-charge
#define SSD1325_SETGRAYTABLE 0xB8        //!< Set gray scale table
#define SSD1325_SETPRECHARGEVOLTAGE 0xBC //!< Set precharge voltage
#define SSD1325_SETVCOMLEVEL 0xBE        //!< Set Vcomh Voltage
#define SSD1325_SETVSL 0xBF              //!< Set segment low voltage (VSL)

#define SSD1325_GFXACCEL 0x23 //!< Graphic acceleration command options
#define SSD1325_DRAWRECT 0x24 //!< Draw rectangle
#define SSD1325_COPY 0x25     //!< Copy

/*!
 * @brief Class that stores state and functions for interacting with SSD1325
 * OLED displays.
 */
class Adafruit_SSD1325 : public Adafruit_GFX {
public:
  /*!
   * @brief Constructor for SSD1325 display
   * @param SID MOSI (microcontroller out, secondary in) pin
   * @param SCLK Serial clock pin
   * @param DC Data/command pin
   * @param RST Reset pin
   * @param CS Chip-select pin
   */
  Adafruit_SSD1325(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS)
      : Adafruit_GFX(128, 64), sid(SID), sclk(SCLK), dc(DC), rst(RST), cs(CS) {}

  /*!
   * @brief Constructor for SSD1325 display
   * @param SID MOSI (microcontroller out, secondary in) pin
   * @param SCLK Serial clock pin
   * @param DC Data/command pin
   * @param RST Reset pin
   */
  Adafruit_SSD1325(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST)
      : Adafruit_GFX(128, 64), sid(SID), sclk(SCLK), dc(DC), rst(RST), cs(-1) {}
  /*!
   * @brief Constructor for SSD1325 display
   * @param DC Data/command pin
   * @param RST Reset pin
   * @param CS Chip-select pin
   */
  Adafruit_SSD1325(int8_t DC, int8_t RST, int8_t CS)
      : Adafruit_GFX(128, 64), sid(-1), sclk(-1), dc(DC), rst(RST), cs(CS) {}

  /*!
   * @brief Allocate and initialize display
   */
  void begin(void);
  /*!
   * @brief Issue single command to SSD1325
   * @param c Command to issue
   */
  void command(uint8_t c);
  /*!
   * @brief Send data to SSD1325
   * @param c Data to send
   */
  void data(uint8_t c);

  /*!
   * @brief High-level command to clear the display
   */
  void clearDisplay(void);
  /*!
   * @brief High-level command that enables or disables invert mode (WoB vs BoW)
   * @param i Whether or not to switch to invert mode
   */
  void invertDisplay(uint8_t i);
  /*!
   * @brief Sets the display brightness
   * @param i Brightness to set
   */
  void setBrightness(uint8_t i);
  /*!
   * @brief Push data currently in RAM to SSD1325 display
   */
  void display();

  /*!
   * @brief Set/clear/invert a single pixel
   * @param x Column of display
   * @param y Row of display
   * @param color Pixel color
   */
  void drawPixel(int16_t x, int16_t y, uint16_t color);

private:
  int8_t sid, sclk, dc, rst, cs;
  void spixfer(uint8_t x);
#ifdef __AVR__
  volatile uint8_t *mosiport, *clkport;
  uint8_t mosipinmask, clkpinmask;
#endif
};

#endif
