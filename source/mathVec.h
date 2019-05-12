#pragma once
#ifndef mathVec_H
#define mathVec_H

#include <iostream>
#include <switch.h>
#include <limits>
#include <cmath>

struct mPoint {
    //public:
    int x;
    int y;
    mPoint(int X = 0, int Y = 0): x(X), y(Y) {}
    u32 length();
};

class mVec {
  public:
    float x;
    float y;
    //mVec(int X,int Y);
    mVec(float X, float Y);
    mVec(mPoint);
    mVec();
    float length();
    void normalize();
    void addVec(mVec);
    void subVec(mVec);
    void scale(float scalar);
    void pCCW();
    void pCW();
    void set(mPoint);
    void set(float X, float Y);
    float dotVec(mVec);
};


#endif
