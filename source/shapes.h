#pragma once
#ifndef shapes_H
#define shapes_H

#include <string>
#include <sstream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <tuple>

#include "mathVec.h"
#include "sDraw.h"
#include "json.h"
#include "fonts.h"
#include "jpeg_decoder.h"
#include "iconUtils.h"
#include "eventUtil.h"
//#include "settings.h"
/*extern "C" {
    #include <switch/services/fs.h>
    #include <switch/services/lr.h>
    #include <switch/services/ncm.h>
    #include <switch/types.h>
    //#include <switch/services/set.h>
}*/
//class eventContainer;
class menu;
class settingMenu;
class mainSettingMenu;
#include "menu.h"



//#include "controls.h"

class myScr;

enum regions {USA = 0, EUR, JAP};
class mPoint;

class scrObject {
    //base should have a position point and a layer number
  public:

    mPoint pos;
    u32 layer;

    bool canSelect;
    bool isVisible;
    bool isSelected;
    bool isHovered;
    bool canHover;
    bool hasLogic;

    virtual void draw(menu* mnu) {};
    virtual void logic(menu*) {};
    scrObject();
    virtual ~scrObject() = default;
    scrObject(mPoint inPt);//: pos(inPt){};
    //    scrObject(mPoint inPt  = mPoint());


};

class polygonShape: public scrObject {
  public:
    u32 color;
    mPoint* points;
    unsigned int pointCount;

    polygonShape(mPoint inputPoints[], unsigned int inPointCount);
    ~polygonShape();
    void draw();

};
class settingsButton: public scrObject {
  public:
    settingsButton();
    settingsButton(mPoint position);
    static const u32 size;
    static const u32 borderColor;
    static const u32 innerColor;
    static const u32 innerColorHovered;
    menu* setMenu;
    //mainSettingMenu* setMenu;
    void draw(menu* mnu);
    virtual void logic(menu*);
    ~settingsButton() {
        //delete settingMenu;
    }
};

class mainSettingsButton: public settingsButton {
  public:
    mainSettingsButton();
    mainSettingsButton(mPoint position);
    void logic(menu*);
    ~mainSettingsButton() {}
};




class settingItem: public scrObject {
  public:
    static const u32 bottomMargin;
    static const u32 borderHeight;
    u32 width;
    u32 height;
    u32 padding;
    u32 gbColor;
    u32 borderColor;
    u32 smallFntColor;
    u32 largeFntColor;

    jsonDataTypeObject jsonObj;
    settingItem();
    settingItem(mPoint position);
    settingItem(u32 wid);
    void draw(menu* mnu);
    virtual ~settingItem() = default;
};



class gameCard: public scrObject {
  public:
    std::string title;
    u64 gameTitleId;
    bool isToggleGamecardIcon;
    regions region;//never needed
    std::string iconPath;//no longer needed
    int updateIcon();
    gameCard(u64 gameTitleID);
    gameCard(u64 gameTitleID,mPoint position);
    unsigned int listPosId;
    unsigned int settingsFileArrayPosId;
    void draw(menu* mnu);
    void logic(menu* mnu);
    int updateIcon(menu* mnu);
    static u32 borderColor;
    static const u32 innerRectColor;
    static const u32 height;
    static const u32 width;
    static const u32 sideBorderThick ;
    static const u32 topBorderThick;
    static const u32 bottomBorderThick;
    static const u32 innerRectTopColor;
    u32 triColor;
    u32 hoveredTriColor;
    Jpeg::Decoder* iconImg;
    ~gameCard();
};

class modItem: public scrObject {
  public:
    u64 gameTitleId;
    bool isMoreOptionsButton;
    enum modTypes{ROMFS,EXEFS};
    modTypes modType;
    int updateIcon(std::string filePath);
    modItem();
//    modItem(u64 gameTitleID,mPoint position);
    
    unsigned int settingsFileArrayPosId;
    std::string modName,folderName,iconPathName,description,versionString,updateUrl;
    void draw(menu* mnu);
    void logic(menu* mnu);//options to select/enable mod 
    int setArrayPos;
    static u32 borderColor;
    static const u32 hoveredBorderColor;
    static const u32 enabledBorderColor;
    static const u32 height;
    static const u32 width;

    static const u32 innerRectColor;
    
    static const u32 borderThick ;

    Jpeg::Decoder* iconImg;
    ~modItem();
};


class gameInfoObj: public scrObject {
  public:

    gameInfoObj(u32 hei, u32 wid);
    gameInfoObj(u32 hei, u32 wid, mPoint position);

    u32 borderColor;
    u32 innerColor;
    u32 height;
    u32 width;

    std::string title;
    std::string gameIdHexString;
    bool isFsModsEnabled,isExeFsModsEnabled;
    int fsModCount,exefsModCount;
    
    void draw(menu* mnu);
    void logic(menu* mnu) {};
};




class simpleOption: public scrObject {
  public:

    simpleOption();
    simpleOption(mPoint position);

    u32 borderColor;
    u32 innerColor;
    u32 height;
    u32 width;
    u32 lineColor;
    u32 hoveredTextColor;
    u32 clickedTextColor;
    u32 defaultTextColor;
    int lineHeight;
    std::string text;
    std::string fontName;
    eventContainer* eventFunctor;

    void draw(menu* mnu);
    void logic(menu* mnu);
    ~simpleOption() {
        delete eventFunctor;
        //  ~scrObject();
    };
};












#endif
