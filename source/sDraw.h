#pragma once
#ifndef sDraw_H
#define sDraw_H


#ifndef DEFAULT_FRAMERATE
    #define DEFAULT_FRAMERATE 60
#endif

#include <switch.h>
#include <iostream>
#include <cmath>//for at least floor
#include <math.h>// for round()
//#include <time.h>
#include <string>
#include <sstream>

#include "json.h"
#include "fonts.h"
#include "upng.h"

enum textColors {BLACK = 0, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};

using std::max;
using std::min;

#include "mathVec.h"
#include "shapes.h"


//need to do some testing
#define FB_WIDTH  1280
#define FB_HEIGHT 720

class polygonShape;

class myScr {
  public:
    unsigned int frameRate; // in frames per second
    u32 width, height;
    u32* framebuf;
    u32 stride;
    
    NWindow* win;// = nwindowGetDefault();
    Framebuffer fb;
    
    ~myScr() {
        //gfxExit();
        framebufferEnd(&fb);//Finishes drawing last frame then ends buff. Not sure if needed but im doing it this way as its most likley not going to break anything and I dont like doing work
        framebufferClose(&fb);
    } // assumes only one screen
    myScr(): frameRate(DEFAULT_FRAMERATE) {
        //gfxInitDefault();
        width=FB_WIDTH;
        height=FB_HEIGHT;
        NWindow* win = nwindowGetDefault();
        //may want to set width to win.width ...
        framebufferCreate(&fb, win, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
        framebufferMakeLinear(&fb);
        //starts drawing first frame as soon as it is declared
        framebuf = (u32*) framebufferBegin(&fb, &stride);
        
        //consoleInit(NULL);
        //std::cout << CONSOLE_ESC(2J);
        //framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);
    }

    void updateScreen();
    
    void drawPixel(mPoint pt, u32 color);
    void drawPixel(mVec pt, u32 color);
    void drawLine(mPoint pt1, mPoint pt2, u32 color);
    void drawLine(mVec pt1, mVec pt2, u32 color);
    void drawRect(mPoint pt1, mPoint pt2, u32 color);
    void drawRectEmpty(mPoint pt1, mPoint pt2, u32 color);
    void drawRectBorder(mPoint pt1, mPoint pt2, u32 borderSize, u32 color);
    void drawCircle(mPoint pt1, int radius, u32);
    void drawPoly(polygonShape* poly, u32 color);
    void background(u32 color);
    void drawRoundedRect(mPoint pt1, mPoint pt2, int radius, u32 color);
    void drawTextLine(std::string txt, mPoint StartPos, font* fnt, u32 color);
    void drawText(std::string txt, mPoint StartPos, font* fnt, u32 maxWidth, bool isCentered, u32 color);
    void drawTriangle(mPoint pt1, mPoint pt2, mPoint pt3, u32 color);
    void drawRGB8(mPoint pt, unsigned char* buf, unsigned int imgBufWid, unsigned int imgBufHei, unsigned int newWid,
                  unsigned int newHei);
    void drawUpdateProgress(double finished,double totoal,font* fnt);
    //Each pixel is 4-bytes due to RGBA8888.

};
    int getTextHeight(std::string txt, font* fnt, u32 maxWidth, bool isCentered);

#endif
