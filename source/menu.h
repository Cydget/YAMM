#pragma once
#ifndef menu_H
#define menu_H
#include <iostream>
#include <sstream>
#include <switch.h>
#include <string>

//for random
#include <ctime>
#include <cstdlib>
#include <tuple>
#include <vector>
#include <algorithm>
#include "eventUtil.h"
#include "mathVec.h"
#include "sDraw.h"
#include "settings.h"
class scrObject;
class gameCard;
//class eventContainer;
#include "shapes.h"
class controller;
#include "controls.h"
#include "fonts.h"
#include <map>
class menu {
public:
    menu* oldMenuPtr;
    menu** activeMenuPtr;
    int id;
    bool isDisplayed;
    unsigned int hoveredId;
    u32 bgColor;
    std::vector<scrObject*>ScreenObjects;
    myScr* scr;
    controller* mainController;
    mPoint curPos;
    fontSetMap* fontMap;

    u32 menuWid;
    u32 menuHei;
    bool drawBGBool;
    menu(menu* oldMenuInput);
    menu(int ID, myScr* sc, controller* control, fontSetMap* fnt);
    virtual ~menu();
    void deleteAbove();
    virtual void draw();
    virtual void logic() {};
    virtual void dummy(){};
    void deleteMenuAndGoToPrevMenu();
    //todo move these over
    void hoverCorrectScreenObjFromJoyConIfNeeded();
    void hoverCorrectScreenObjFromTouchIfNeeded();
};







class settingMenu: public menu {
  public:
    settingMenu(menu* oldMenuInput);
    ~settingMenu() = default;
    virtual void logic() {};
};

class mainSettingMenu: public settingMenu {
  public:
    mainSettingMenu(menu* oldMenuInput);
    ~mainSettingMenu() = default;
    void logic();
};

class gameSelectMenu: public menu {
  public:
    gameSelectMenu(int ID, myScr* sc, controller* control, fontSetMap* fnt);
    ~gameSelectMenu();
    
    int gameRowCount;

    static const int gapBetweenGameCards;// = gameCard::width / 2 / 4;
    unsigned int gameCardCount;
    void centerGameCardsIfNeeded();
    void addGameCard(gameCard* gC);
    void toggleHoveredGameCardScreen();
    u32 getWidthOfGameCds();
    bool displayVis;
    void moveGCDS(int amount);
    void logic();
    enum menuMode{NORMAL,MOVING_GC,DEBUG};
    gameCard * lastHoveredGCPtr;
    menuMode mode;
    void enableGameMoving(){
        mode = MOVING_GC;
    }
    void swapGCs(gameCard *oGC,gameCard * nGC);
    void addVisibleGames();
    void addHiddenGames();
    void removeAllGameCards();
    void toggleVisibilityMode();
    void addUndiscoveredGames();
    void createFsModMenu();
    void createExeFsModMenu();
    //void draw();
};


class modSelectMenu: public menu {
  public:
//    modSelectMenu(int ID, myScr* sc, controller* control, fontSetMap* fnt);
    modSelectMenu(menu* oldMenuInput,std::string modType,u64 gamEID);
    //~modSelectMenu();
        ~modSelectMenu() override = default;// = default;

    static const int gapBetweenMods;// = gameCard::width / 2 / 4;
    std::string menuModType;//should be settings::fsModsKey or  settings::exeModsKey thats just the "Standard" that Im using for this program
    void deleteHoveredMod();
    void moveMods(int amount);
    void logic();
    void addMods();
    void swapMods();
    void swapModsLeft();
    void swapModsRight();
    
    unsigned int gameCardCount;
    void centerGameCardsIfNeeded();
    void addGameCard(gameCard* gC);
    
    u32 getWidthOfGameCds();
    bool displayVis;

    u64 gameID;
    enum menuMode{NORMAL,MOVING_GC,DEBUG};
    gameCard * lastHoveredGCPtr;
    menuMode mode;
    void enableGameMoving(){
        mode = MOVING_GC;
    }
    void swapGCs(gameCard *oGC,gameCard * nGC);
    void addVisibleGames();
    void addHiddenGames();
    void removeAllGameCards();
    void toggleVisibilityMode();
    void addUndiscoveredGames();
    //void draw();
};





class inputMenu: public menu{
public:
    int radius;
    std::vector<std::tuple<std::string,std::string,eventContainer*>> buttonOptions;
    inputMenu(menu* oldMenuInput);
    int fontPadding;
    ~inputMenu() override = default;// = default;
    u32 boxColor;
    void draw();
    void logic();
    void setOptions(std::vector<std::tuple<std::string,std::string,eventContainer*>> &inputOptions);
    void turnOptionsIntoScreenObjects();
};
//convertToHexString(uint64_t value);



#endif
