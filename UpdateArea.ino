/*

  UpdateArea.ino

  Demonstration for the UpdateDisplayArea command

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 19, /* dc=*/ 5, /* reset=*/ 22);


int16_t offset;				// current offset for the scrolling text
u8g2_uint_t width;			// pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined
const char *text = "eddy Electronics";	// scroll this text from right to left

const uint8_t tile_area_x_pos = 2;	// Update area left position (in tiles)
const uint8_t tile_area_y_pos = 3;	// Update area upper position (distance from top in tiles)
const uint8_t tile_area_width = 12;
const uint8_t tile_area_height = 3;	// this will allow cour18 chars to fit into the area

const u8g2_uint_t pixel_area_x_pos = tile_area_x_pos * 8;
const u8g2_uint_t pixel_area_y_pos = tile_area_y_pos * 8;
const u8g2_uint_t pixel_area_width = tile_area_width * 8;
const u8g2_uint_t pixel_area_height = tile_area_height * 8;

void setup(void) {
  u8g2.begin();

  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_helvR10_tr);	// choose a suitable font
  u8g2.drawStr(0, 12, "UpdateDisplayArea");	// write something to the internal memory

  // draw a frame, only the content within the frame will be updated
  // the frame is never drawn again, but will stay on the display
  u8g2.drawBox(pixel_area_x_pos - 1, pixel_area_y_pos - 1, pixel_area_width + 2, pixel_area_height + 2);

  u8g2.sendBuffer();					// transfer internal memory to the display

  u8g2.setFont(u8g2_font_courB18_tr);	// set the target font for the text width calculation
  width = u8g2.getUTF8Width(text);		// calculate the pixel width of the text
  offset = width + pixel_area_width;
  u8g2.setContrast(50);


}

void loop(void) {
  u8g2.clearBuffer();						// clear the complete internal memory

  // draw the scrolling text at current offset
  u8g2.setFont(u8g2_font_courB18_tr);		// set the target font
  u8g2.drawUTF8(
    pixel_area_x_pos - width + offset,
    pixel_area_y_pos + pixel_area_height + u8g2.getDescent() - 1,
    text);								// draw the scolling text

  // now only update the selected area, the rest of the display content is not changed
  u8g2.updateDisplayArea(tile_area_x_pos, tile_area_y_pos, tile_area_width, tile_area_height);

  offset--;								// scroll by one pixel
  if ( offset == 0 )
    offset = width + pixel_area_width;			// start over again

  delay(10);							// do some small delay
}
