#include "controls.h"



controller::controller(): scansPerSecond(1) {
    //    changedSinceLastPoll = false;
    drawAll = false;
    hidScanInput();
    closeHB = false;
    hidJoystickRead(&posLeftStick, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
    tmp = 0;
    lastScanTime = clock();
    hidScanInput();
    //        lastScanTime = clock();

    //	changedSinceLastPoll = true;
    hidJoystickRead(&posLeftStick, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
    hidJoystickRead(&posRightStick, CONTROLLER_P1_AUTO, JOYSTICK_RIGHT);
    kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
    kUp = hidKeysUp(CONTROLLER_P1_AUTO);
    specialEvent = 0;
    touchCount = 0;
    touchCount = hidTouchCount();

    if(touchCount > 0) {
        hidTouchRead(&touch, touchCount - 1);
    }


}

void controller::scan() {
    //  if(tmp>10){
    //	tmp=0;
    //    if ( (double)( clock() - lastScanTime)/(CLOCKS_PER_SEC) > (double)(1/(scansPerSecond)) ) {
    hidScanInput();
    //        lastScanTime = clock();

    //	changedSinceLastPoll = true;
    hidJoystickRead(&posLeftStick, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
    hidJoystickRead(&posRightStick, CONTROLLER_P1_AUTO, JOYSTICK_RIGHT);
    kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
    kUp = hidKeysUp(CONTROLLER_P1_AUTO);
    //  }else {tmp++;}
    u32 newTouchCt = hidTouchCount();


    if(newTouchCt > 0) {
        hidTouchRead(&touch, newTouchCt - 1);
        //This works, but it makes it so that it will "press" a on something even if it is far away
        // if(newTouchCt!=touchCount){kDown |= KEY_A;}
        kHeld |= KEY_A;
    }

    touchCount = newTouchCt;
}

float controller::getLSAngle() {
    float theata = 0;
    mPoint vec(1, 0); //unit vector
    //thata=(posLeftStick.dx * mPoint.x  +  posLeftStick.dy * mPoint.y)/std::scrt(posLeftStick.dx*posLeftStick.dx+posLeftStick.dy*posLeftStick.dy) * vec.length())
    return theata;
}
unsigned int controller::getClosestPoint(mVec cPos, std::vector<scrObject*> const& ScreenObjects, unsigned int oldId,
        myScr* wtf) {
    /*
    maybe make pVecs[] a const
    cPos should be current cursor pos
    this doesnt technically need to belong to controller, i might move it later to someplace else, but for now it is fine

     */
    //todo change wtf to scr

    //add a check to see if it is on screen or close to being on screen

    mVec joyVec(this->posLeftStick.dx, -1 * (this->posLeftStick.dy)); // for inverted controls make this positive
    //joyVec.x +=     this->posRightStick.dx;   //for both joystick support un comment this
    //joyVec.y += -1*(this->posRightStick.dy);
    joyVec.normalize();

    if(joyVec.length() > 0.1) {
        mVec drawVec = joyVec;
        mVec drawVecPerp = joyVec;
        //    drawVecPerp.pCCW();
        drawVecPerp.scale(-30);
        drawVec.scale(30);

        drawVec.addVec(cPos);
        drawVecPerp.addVec(cPos);
        wtf->drawLine(cPos, drawVec, RGBA8_MAXALPHA(0, 0, 255)); //draw line showing line
        wtf->drawLine(cPos, drawVecPerp, RGBA8_MAXALPHA(255, 255, 0)); //draw line showing line
        //wtf->drawLine(cPos,joyVec,RGBA8_MAXALPHA(0,0,255)); //draw line showing line
        //wtf->drawPixel(drawVec,RGBA8_MAXALPHA(0,255,255)); // draw at 0,0
        wtf->drawPixel(drawVec, RGBA8_MAXALPHA(255, 0, 0));  // draw normal vec point
        wtf->drawPixel(cPos, RGBA8_MAXALPHA(255, 0, 255));  //draw selected center point


        joyVec.normalize();
        float closestDistanceToLine = 2000;//std::numeric_limits<float>::max(); //this is closest distance to line
        float closestDistanceToPoint = 2000;
        std::numeric_limits<float>::max();
        unsigned int retId = oldId;

        mVec quadVec = joyVec; //vec used to check quadrent
        joyVec.pCCW();

        for(size_t i = 0; i < ScreenObjects.size(); ++i) {
            //excludes if too far(40 px) off screen
            int offscreenfudge = 250;

            if(ScreenObjects[i]->canHover && i != oldId && (ScreenObjects[i]->pos.x >=  0 - offscreenfudge &&
                    ScreenObjects[i]->pos.x <= int(wtf->width) + offscreenfudge) && (ScreenObjects[i]->pos.y >=  0 - offscreenfudge &&
                            ScreenObjects[i]->pos.y <= int(wtf->height) + 40)) {

                mVec q(ScreenObjects[i]->pos);
                wtf->drawLine(cPos, q, RGBA8_MAXALPHA(0, 0, 255)); //draw line showing line

                q.subVec(cPos);

                float lastCheckedDistanceToLine = std::abs(q.dotVec(joyVec));    //std::abs(q.dotVec(joyVec));

                wtf->drawLine(ScreenObjects[i]->pos, mVec(ScreenObjects[i]->pos.x, ScreenObjects[i]->pos.y - lastCheckedDistanceToLine),
                              RGBA8_MAXALPHA(0, 255, 0));


                // if(i != oldId) { // the old id should have a distance of 0 which is unbeatable

                if((lastCheckedDistanceToLine - closestDistanceToLine  < 5)) {    //the 20 is there, to get some other information
                    float lastCheckedDistanceToPoint = q.length();

                    if(closestDistanceToPoint > lastCheckedDistanceToPoint) {

                        //wtf->drawLine(pVecs[i], mVec(pVecs[i].x, pVecs[i].y - lastCheckedDistanceToLine), RGBA8_MAXALPHA(0, 255, 0));

                        if((quadVec.y >= 0 && q.y >= -30 && quadVec.x >= 0 && q.x >= -30) ||      //at least quad 1
                           (quadVec.y >= 0 && q.y >= -30 && quadVec.x <= 0 && q.x <= 30) ||    //at least quad 2
                           (quadVec.y <= 0 && q.y <= 30 && quadVec.x <= 0 && q.x <= 30) ||   //at least quad 3
                           (quadVec.y <= 0 && q.y <= 30 && quadVec.x >= 0 && q.x >= -30)      //at least quad 4
                          ) {
                            //the 30's are there, because we dont want to limit to exactly certain quadrents as that would make it a little off

                            //The quad check needs to be changed to be
                            closestDistanceToLine  = lastCheckedDistanceToLine;
                            closestDistanceToPoint = lastCheckedDistanceToPoint;
                            retId = i;
                        }
                    }
                }

                // } // end old id check
            }


        }//for loop ending

        return retId;
    }
    else {return oldId;}
}


void controller::exitHomebrewEvent() {
    this->closeHB = true;
}






void controller::checkForEventTrigger() {
    //wtf->drawRect(mPoint(800, 20), mPoint(300, 100), RGBA8_MAXALPHA(0, 255, 35));

    if((kDown & KEY_PLUS) || kHeld & KEY_PLUS) {
        exitHomebrewEvent();
    }

}



