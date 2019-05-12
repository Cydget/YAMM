#pragma once
#ifndef controls_H
#define controls_H

#include <time.h>
#include <iostream>
#include <switch.h>
#include <cmath>
#include <limits>
#include <vector>
#include "mathVec.h"
class myScr;
#include "sDraw.h"
class scrObject;
#include "shapes.h"


class controller {
  public:
    JoystickPosition posLeftStick, posRightStick;
    bool closeHB;
    bool drawAll;
    double scansPerSecond;
    //bool changedSinceLastPoll;
    clock_t lastScanTime;
    unsigned int tmp;
    u64 kDown, kHeld, kUp;
    touchPosition touch;
    u32 touchCount;
    u32 specialEvent;
    void scan();
    float getLSAngle();
    float getRSAngle();
    //    unsigned int getClosestPoint(mVec m, mVec pVecs[], unsigned int pVecCount, unsigned int oldId, myScr* wtf);
    unsigned int getClosestPoint(mVec cPos, std::vector<scrObject*> const& ScreenObjects, unsigned int oldId, myScr* wtf);
    void checkForEventTrigger();
    // events and triggers

    void exitHomebrewEvent();
    //void select
    controller();
};


#endif
