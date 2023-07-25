#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 19, /* dc=*/ 5, /* reset=*/ 22);

float gs_rad; //stores angle from where to start in radinats
float ge_rad; //stores angle where to stop in radinats

byte cx=64; //x center
byte cy=45; //y center
byte radius=40; //radius
byte radius2=30; //radius
byte percent=80; //needle percent
int pot = 0;

void Drawgauge(int x, byte y, byte r, byte p, int v, int minVal, int maxVal) {
  u8g2.firstPage();
  do { 
    u8g2.drawCircle(x,y,r, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT );
    u8g2.drawCircle(x,y,radius2, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT );
    u8g2.drawLine(cx-r, cy, cx-radius2, cy);
    u8g2.drawLine(cx+r, cy, cx+radius2, cy);
    u8g2.setDrawColor(1);
    u8g2.drawStr(x-30,y+15,"Value:");
    u8g2.setCursor(x+7,y+15);
    u8g2.print(pot);
    float val = map(pot, 0, 1020, 0, 180);
    val = val*3.14/180 -1.572;
    int xp = x+(sin(val) * radius);
    int yp = y-(cos(val) * radius);
    u8g2.drawLine(x,y,xp,yp);
    u8g2.drawLine(x-1,y-1,xp,yp);
    u8g2.drawLine(x+1,y-1,xp,yp);
    u8g2.drawLine(x-1,y,xp,yp);
    u8g2.drawLine(x+1,y,xp,yp);
  } while ( u8g2.nextPage() );
}

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_12b_te);
  u8g2.setDisplayRotation(U8G2_R2);
}

void loop() {
  pot = analogRead(34);
  Drawgauge(cx,cy,radius,percent,pot,0,100);

}
