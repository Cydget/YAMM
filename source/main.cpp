//#include <string.h>
//#include <stdio.h>
#include <switch.h>

//not used yet
#ifndef DEFAULT_FRAMERATE
    #define DEFAULT_FRAMERATE 60
#endif

#include <time.h>
#include <iostream>
#include <string>
#include <cmath>
#include "sDraw.h"
#include "shapes.h"
#include "mathVec.h"
#include "controls.h"
#include "menu.h"
#include "settings.h"
#include <ctime>
#include <cstdlib>
#include <map>

extern "C" {
#include <switch/services/fs.h>
#include <switch/services/lr.h>
#include <switch/services/ncm.h>
#include <switch/types.h>
    //#include <switch/services/set.h>
}

using namespace std;

//static settings menu::globalSettings("settings path ");

int main(int argc, char** argv) {
    gSettings::getInstance();
    gSettings::getInstance().main->addGame(0x010021C000B6A000,"Binding of dank");
    //gSettings::getInstance().main->addMod(0x010021C000B6A000,settings::fsModsKey, "Trash","TrashFolder", "demo.png","This is a bad description");
    //gSettings::getInstance().main->setGameVisibility(0x010021C000B6A000,true);
    romfsInit();
    myScr* mainScr = new myScr();
    controller* myController = new controller();
    //todo Move this to fonts.h
    //todo find out why I cant just insert a font into the map directly without it crashing on exit


    fontSetMap fontMap;
    font fnts[8] = {
        {"romfs:/fonts/noto_sans_symbols_regular_14.json"},
        {"romfs:/fonts/noto_sans_symbols_regular_24.json"},
        {"romfs:/fonts/noto_sans_symbols_regular_48.json"},
        {"romfs:/fonts/noto_sans_symbols_regular_65.json"},
        {"romfs:/fonts/noto_sans_ui_bold_14.json"},
        {"romfs:/fonts/noto_sans_ui_bold_24.json"},
        {"romfs:/fonts/noto_sans_ui_bold_48.json"},
        {"romfs:/fonts/noto_sans_ui_bold_65.json"},
    };
    std::string fontNames[8] = {"symbols14", "symbols24", "symbols48", "symbols65", "sans14", "sans24", "sans48", "sans65"};

    for(unsigned int i = 0; i < sizeof(fontNames) / sizeof(fontNames[0]); i++) {
        fontMap.insert(fontSetMap::value_type(fontNames[i], &fnts[i]));
    }



    //font mainFont("freeSansReg", "romfs:/freesans_regular_14.json");

    menu* activeMenu =  new gameSelectMenu(0, mainScr, myController, &fontMap);
    //god this line is beautiful and nececessary
    activeMenu->activeMenuPtr = &activeMenu;
    //now I can do things like
    // activeMenu->activeMenuPt->activeMenu->activeMenuPt->activeMenu->activeMenuPt->activeMenu->activeMenuPt->draw()


    while(appletMainLoop()) {

        myController->scan();
        myController->checkForEventTrigger();//does this globably

        if(myController->closeHB) { break; }  //global break

        activeMenu->draw();
        activeMenu->logic();
   //     for(f=t/2;f<t;f+=100){
       //     activeMenu->scr->drawUpdateProgress(f,t,activeMenu->fontMap->find("sans24")->second);
     //   }
        mainScr->updateScreen();

    }

    activeMenu->deleteAbove(); //needed if exited from inner menu using (+)
    delete activeMenu;
    delete mainScr;
    delete myController;

    romfsExit();
    return 0;
}
