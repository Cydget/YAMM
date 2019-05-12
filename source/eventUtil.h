#pragma once

#ifndef eventUtil_H
#define eventUtil_H

//The purpose of having this file, is due to the fact that I would have to forward declare eventContainer in shapes.h
//But that is an issue as it is an incomplete type
//Also dont want that issue to appear elsewhere

class menu;

class eventContainer {
  public:
    eventContainer() {};
    virtual void trigger() = 0;
    virtual void operator()() = 0;
    virtual ~eventContainer() = default;
};

class menuEvent: public eventContainer {
  public:
    void(menu::*meathod)();
    menu* obj;
    menuEvent(menu* objInput, void(menu::*inputMeathod)()): meathod(inputMeathod), obj(objInput) {}
    void trigger() {
        (obj->*meathod)();
    }
    void operator()() {
        (obj->*meathod)();
    }
};
//I dont think menu.h is needed at all
#include "menu.h"
#endif