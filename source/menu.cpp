#include "menu.h"

const int gameSelectMenu::gapBetweenGameCards = gameCard::width / 2 / 4;
/*Todo make a bunch of functions to fix gamecard stuff*/

menu::menu(int ID, myScr* sc, controller* control, fontSetMap* fnt):
    //this will be called once for the main menu the other will be used all other times
    id(ID),
    isDisplayed(true),
    hoveredId(0),
    bgColor(RGBA8_MAXALPHA(250, 231, 215)),
    //    totalSelectableObjCount(0),
    scr(sc),
    mainController(control),
    curPos(scr->width / 2, 0),
    fontMap(fnt),
    menuWid(scr->width),
    menuHei(scr->height), drawBGBool(true) {}

menu::menu(menu* oldMenuInput):
    oldMenuPtr(oldMenuInput),
    activeMenuPtr(oldMenuInput->activeMenuPtr),
    id(oldMenuInput->id + 1),//very important to see how many levels deep we are
    isDisplayed(true),
    hoveredId(0),
    bgColor(RGBA8_MAXALPHA(250, 231, 215)),
    scr(oldMenuInput->scr),
    mainController(oldMenuInput->mainController),
    curPos(scr->width / 2, 0),//not sure why I made this decision
    fontMap(oldMenuInput->fontMap), menuWid(scr->width),
    menuHei(scr->height), drawBGBool(true) {
    *(this->activeMenuPtr) = this;
}

menu::~menu() {
    for(size_t i = 0; i < ScreenObjects.size(); ++i) {
        delete ScreenObjects[i];
    }
}

void menu::deleteAbove() {
    if(id > 0) {
        oldMenuPtr->deleteAbove();
        delete oldMenuPtr;
    }
}
void menu::deleteMenuAndGoToPrevMenu(){
    if((*(this->activeMenuPtr))->id>0){
        //May consider createing a function to draw all menus above in order from id 0 -> current id, that way if there are multi level menus it will look perfectly fine assuming that upper level menus were not changed
        menu* currentActiveMenuPtrVal = *(this->activeMenuPtr);
        *(this->activeMenuPtr) = (*(this->activeMenuPtr))->oldMenuPtr; //this->oldMenuPtr;
        delete currentActiveMenuPtrVal;
    }
}
void menu::draw() {
    if(drawBGBool) {
        scr->background(bgColor);
    }

    for(size_t i = 0; i < ScreenObjects.size(); ++i) {
        if(ScreenObjects[i]->pos.x > -150 && ScreenObjects[i]->pos.x < int(scr->width) + 150 && ScreenObjects[i]->pos.y > -50 &&
           ScreenObjects[i]->pos.y < int(scr->height) + 50)
        { ScreenObjects[i]->draw(this); }

        //passing this, so that each object has access to important things like fonts and stuff like controllers
    }

}
void menu::hoverCorrectScreenObjFromJoyConIfNeeded(){
     if(std::abs(mainController->posLeftStick.dx) || std::abs(mainController->posLeftStick.dy)) {
        ScreenObjects[hoveredId]->isHovered = false;
        hoveredId = mainController->getClosestPoint(ScreenObjects[hoveredId]->pos, ScreenObjects, hoveredId, scr);
        ScreenObjects[hoveredId]->isHovered = true;
    }
}
void menu::hoverCorrectScreenObjFromTouchIfNeeded(){
            if(mainController->touchCount > 0) {
            scr->drawCircle(mPoint(mainController->touch.px, mainController->touch.py), 10, RGBA8_MAXALPHA(255, 0, 0));
            ScreenObjects[hoveredId]->isHovered = false;
            //std::sqrt(this->x * this->x + this->y * this->y);
            float smallLen =   mVec(mainController->touch.px - ScreenObjects[hoveredId]->pos.x,
                                    mainController->touch.py - ScreenObjects[hoveredId]->pos.y).length();

            for(size_t i = 0; i < ScreenObjects.size(); i++) {
                float tmpLen = mVec(mainController->touch.px - float(ScreenObjects[i]->pos.x),
                                    mainController->touch.py - float(ScreenObjects[i]->pos.y)).length();

                if(ScreenObjects[i]->canHover && abs(tmpLen) < abs(smallLen)) { //ScreenObjects[hoveredId]->pos
                    smallLen = tmpLen;
                    hoveredId = i;
                }
            }

            ScreenObjects[hoveredId]->isHovered = true;
            //controller sets key a to down
        }
}
void gameSelectMenu::createFsModMenu(){
    
    if(dynamic_cast<gameCard*>(this->ScreenObjects[this->hoveredId])!=nullptr){
        u64 passGameId=dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId;
        /*modSelectMenu* modMenu = */new modSelectMenu(this,settings::fsModsKey,passGameId);
//modMenu->menuModType = settings::fsModsKey;
        //modMenu->gameID = dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId;//passGameId;//passGameId;
    }
}
void gameSelectMenu::createExeFsModMenu(){
    if(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])!=nullptr){
        u64 passGameId=dynamic_cast<gameCard*>(this->ScreenObjects[this->hoveredId])->gameTitleId;
       // modSelectMenu* modMenu = new modSelectMenu(this);
        //modMenu->menuModType = settings::exeModsKey;
        /*modSelectMenu* modMenu = */new modSelectMenu(this,settings::exeModsKey,passGameId);
           //     modSelectMenu* modMenu = new modSelectMenu(this,settings::exeModsKey,dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId);
//
  //      modMenu->gameID = dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId;
    }
}
void gameSelectMenu::centerGameCardsIfNeeded() {
    /*
    u32 gameCTotalWidth = getWidthOfGameCds();

    if(gameCTotalWidth < scr->width) {
        u32 fixedOffset = (scr->width - gameCTotalWidth) / 2 ;

        //
        for(size_t i = 0, end = ScreenObjects.size(); i < end;  ++i) {
            if(dynamic_cast<gameCard*>(ScreenObjects[i]) != nullptr) {
                ScreenObjects[i]->pos.x += fixedOffset;
            }
        }
    }
    */
}

/*Idea get vector of games ids
 * Then get info my gameId
 * Make 2 lists 
 * 
 * New Idea get vector of visible games-> load them onto screen
 * Replace them with hidden games when button pressed
 * Replace again with new set when unhidden
 * */
void gameSelectMenu::addGameCard(gameCard* gC) {
    int gap = gameSelectMenu::gapBetweenGameCards;
    int wid = gameCard::width;
    gC->listPosId = gameCardCount;
    gameCardCount++;
    ScreenObjects.push_back(gC);
//gameRowCount
    if(gameCardCount == 1) {//not sure why i did this probably centering
        ScreenObjects[ScreenObjects.size() - 1]->pos = 
        mPoint(gap+wid/2,  scr->height - (gameRowCount)*(gameCard::height + gap)+ gameCard::height/2);
    }//maybe add a little extra for sides
    else {
        mPoint lastPoint = ScreenObjects[ScreenObjects.size() - 2]->pos;
        if((gameCardCount%gameRowCount)==1){
            lastPoint.x += wid+gap;
            lastPoint.y -= (gameRowCount-1)*(gameCard::height+gap);
        }
        else{lastPoint.y +=gameCard::height+gap;}
//        lastPoint.x += wid;
       // lastPoint.y += (gameCard::height)*((gameCardCount-1)%(gameRowCount+1));        
        //if((1+gameCardCount)%gameRowCount!=(gameRowCount-1)){

        //}
        //else{
        //lastPoint.y += (gameRowCount-1)*(gap + gameCard::height);
        //lastPoint.x += gap + wid;
        //}
        ScreenObjects[ScreenObjects.size() - 1]->pos = lastPoint;
    }
    /*I need it to snake when putting them in like:
     * 1 4
     * 2 5
     * 3 6
     * */
    
    
}
void gameSelectMenu::toggleHoveredGameCardScreen(){
    if(dynamic_cast<gameCard*>(ScreenObjects[hoveredId]) != nullptr){
        unsigned int lastKnownGCScrObjId=hoveredId;
        //Move selected GC to the end of the list. This assumes that the GCs are in order. This should be a resonable assumption
        for(unsigned int i=(hoveredId+1);i<ScreenObjects.size();i++){
            if( dynamic_cast<gameCard*>(ScreenObjects[i]) != nullptr  ){
                if(!(dynamic_cast<gameCard*>(ScreenObjects[i])->isToggleGamecardIcon) ){
                    gameSelectMenu::swapGCs(dynamic_cast<gameCard*>(ScreenObjects[lastKnownGCScrObjId]),dynamic_cast<gameCard*>(ScreenObjects[i]));
                    std::swap(ScreenObjects[lastKnownGCScrObjId],ScreenObjects[i]);
                    lastKnownGCScrObjId = i;
                }
            }
        }
        //Hide then remove from screensettings::toggleGameVisibility(uint64_t gameID){
        gSettings::getInstance().main->toggleGameVisibility(dynamic_cast<gameCard*>(ScreenObjects[lastKnownGCScrObjId])->gameTitleId);
        ScreenObjects.erase(ScreenObjects.begin()+lastKnownGCScrObjId);
        //
    }
    deleteMenuAndGoToPrevMenu();//This function is meant to only be called when this function is called from a inputMenu 
}
u32 gameSelectMenu::getWidthOfGameCds() {
    int gap = gameSelectMenu::gapBetweenGameCards;
    int wid = gameCard::width;
    return (gameCardCount) * (wid) - wid / 2 + (gameCardCount) * gap + wid / 2 + gap;
    //Position of gamecards is its center
    //count*wid+
    //-wid/2 + //puts it to the left of the gamecard icon
    //count*gap+ //gets the total gap 
    //wid/2+ // puts back to right?
    //gap //adds gap for future game?
    
    
}
void gameSelectMenu::moveGCDS(int amount) {
    for(size_t i = 0, end = ScreenObjects.size(); i < end;  ++i) {
        if(dynamic_cast<gameCard*>(ScreenObjects[i]) != nullptr) {
            ScreenObjects[i]->pos.x += amount;
        }
    }
}
void gameSelectMenu::toggleVisibilityMode(){
    if(!displayVis){
        displayVis=true;
        gameSelectMenu::removeAllGameCards();
        gameSelectMenu::addVisibleGames();
    }
    else{
        displayVis=false;
        gameSelectMenu::removeAllGameCards();
        gameSelectMenu::addHiddenGames();
    }
    hoveredId=0;
}
void gameSelectMenu::removeAllGameCards(){
    for(int i= ScreenObjects.size() -1 ;i>=0;i--){
        if(dynamic_cast<gameCard*>(ScreenObjects[i]) != nullptr ){
            ScreenObjects.erase(ScreenObjects.begin()+i);
        }
    }
        gameCardCount = 0;
}
void gameSelectMenu::addUndiscoveredGames(){
    std::vector<u64>tidList;
    size_t readtidSize = 0;
    
    getTitleList(FsStorageId_GameCard, tidList, readtidSize);
    getTitleList(FsStorageId_NandUser, tidList, readtidSize);
    getTitleList(FsStorageId_SdCard, tidList, readtidSize);
    
    std::sort( tidList.begin(), tidList.end() );
    tidList.erase( std::unique( tidList.begin(), tidList.end() ), tidList.end() );
    
    if(readtidSize > 0) {
        for(int i = 0;i< int(tidList.size());i++){
            gSettings::getInstance().main->addGame(tidList[i],getTitleName(tidList[i]));/*std::string getTitleName(u64 titleID)*/
        }
    }    
}
void gameSelectMenu::addHiddenGames(){
    // do stuff here to import new undiscovered games

    gameSelectMenu::addUndiscoveredGames();
    //back to normal stuffs normal stuff
    gameCardCount = 0;
    //add cards for testing
    //size_t intGameCardCount = gSettings::getInstance().main->getVisibleGamesCount();//getGameCardCount();
    std::vector<uint64_t> gameCardTitleIdVec =gSettings::getInstance().main->getVectorOfHiddenGames();

    for(size_t i = 0; i < gameCardTitleIdVec.size(); ++i) {
        gameSelectMenu::addGameCard(new gameCard(gameCardTitleIdVec[i]));
        dynamic_cast<gameCard*>(ScreenObjects.back())->settingsFileArrayPosId = gSettings::getInstance().main->getGameJsonNumb(gameCardTitleIdVec[i], settings::gamePositionInOrderKey);
    }
    gameSelectMenu::addGameCard(new gameCard(0));
    dynamic_cast<gameCard*>(ScreenObjects.back())->gameCard::updateIcon(this);
    dynamic_cast<gameCard*>(ScreenObjects.back())->isToggleGamecardIcon=true;
    centerGameCardsIfNeeded();
}
void gameSelectMenu::addVisibleGames(){
    gameCardCount = 0;
    //add cards for testing
    //size_t intGameCardCount = gSettings::getInstance().main->getVisibleGamesCount();//getGameCardCount();
    std::vector<uint64_t> gameCardTitleIdVec =gSettings::getInstance().main->getVectorOfVisibleGames();

    for(size_t i = 0; i < gameCardTitleIdVec.size(); ++i) {
        gameSelectMenu::addGameCard(new gameCard(gameCardTitleIdVec[i]));
        dynamic_cast<gameCard*>(ScreenObjects.back())->settingsFileArrayPosId = gSettings::getInstance().main->getGameJsonNumb(gameCardTitleIdVec[i], settings::gamePositionInOrderKey);
    }
    gameSelectMenu::addGameCard(new gameCard(0));
    dynamic_cast<gameCard*>(ScreenObjects.back())->gameCard::updateIcon(this);
        dynamic_cast<gameCard*>(ScreenObjects.back())->isToggleGamecardIcon=true;
    centerGameCardsIfNeeded();
}

gameSelectMenu::gameSelectMenu(int ID, myScr* sc, controller* control, fontSetMap* fnt): menu(ID, sc, control, fnt) {
    displayVis=true;
    mode = NORMAL;
    ScreenObjects.push_back(new mainSettingsButton());
    ScreenObjects[ScreenObjects.size() - 1]->pos = mPoint(settingsButton::size, settingsButton::size);
    gameRowCount=2;

    //Add info box for main menu
    ScreenObjects.push_back(new gameInfoObj(scr->height / 3, scr->width / 2 + 100, mPoint(scr->width / 2,scr->height / 3 / 2
                                            /*scr->height / 3 / 2 + scr->height / 3 / 4*/      )));
    dynamic_cast<gameInfoObj*>(ScreenObjects[ScreenObjects.size() - 1])->title = "No game";
    dynamic_cast<gameInfoObj*>(ScreenObjects[ScreenObjects.size() - 1])->gameIdHexString = "Unknown";
    gameSelectMenu::addVisibleGames();
    //todo add empty gamecard +

    


}


//modSelectMenu::modSelectMenu(menu* oldMenuInput): menu(oldMenuInput) {
    //fontPadding = 8;
    //radius = 40;
    //drawBGBool = false;
    //boxColor = RGBA8_MAXALPHA(128, 120, 0);
    ///    menuWid = xxx;
    ///    menuHei = xxx;
   // menuWid = scr->width / 4;
    //menuHei = scr->height / 4;
//}

void modSelectMenu::addMods(){
    //ScreenObjects
    //gSettings::getInstance().main->importMod("./testImport/");
    size_t modCount = gSettings::getInstance().main->getModCount(gameID,menuModType);//menuModType
    
    std::string modName="",folderName="",iconPathName="",description="",versionString="",updateUrl="";
    
    ScreenObjects.push_back(new mainSettingsButton());
    ScreenObjects[ScreenObjects.size() - 1]->pos = mPoint(settingsButton::size, settingsButton::size);
    
    mPoint startPosition(5+modItem::width/2,scr->height-modItem::height/2 - 5);
    for(unsigned int i=0;i<modCount;i++){
        modItem* tmp = new modItem();
        gSettings::getInstance().main->getModInfo(gameID,
                                                    menuModType,
                                                    i,
                                                    tmp->modName,
                                                    tmp->folderName,
                                                    tmp->iconPathName,
                                                    tmp->description,
                                                    tmp->versionString,
                                                    tmp->updateUrl);
        tmp->updateIcon(tmp->iconPathName);
        tmp->setArrayPos = i;
        tmp->pos = startPosition;
        ScreenObjects.push_back(tmp);
        startPosition.x+=modItem::width;
        //possible error: getModInfo fails due to some error and thus results in last modpacks info ending up in the new modpack
        //This hopefully shouldnt occur because of error checking the file when loading settings from file
    }

}
/*
 * void gameSelectMenu::addVisibleGames(){
    gameCardCount = 0;
    //add cards for testing
    size_t intGameCardCount = gSettings::getInstance().main->getVisibleGamesCount();//getGameCardCount();
    std::vector<uint64_t> gameCardTitleIdVec =gSettings::getInstance().main->getVectorOfVisibleGames();

    for(size_t i = 0; i < gameCardTitleIdVec.size(); ++i) {
        gameSelectMenu::addGameCard(new gameCard(gameCardTitleIdVec[i]));
        dynamic_cast<gameCard*>(ScreenObjects.back())->settingsFileArrayPosId = gSettings::getInstance().main->getGameJsonNumb(gameCardTitleIdVec[i], settings::gamePositionInOrderKey);
    }
    gameSelectMenu::addGameCard(new gameCard(0));
    dynamic_cast<gameCard*>(ScreenObjects.back())->gameCard::updateIcon(this);
        dynamic_cast<gameCard*>(ScreenObjects.back())->isToggleGamecardIcon=true;
    centerGameCardsIfNeeded();
}
 * 
 * */
modSelectMenu::modSelectMenu(menu* oldMenuInput,std::string modType,u64 gamEID): menu(oldMenuInput) {
    menuModType=modType;
    gameID = gamEID;
    createDirectories("./info/"+std::to_string(gamEID));

  //  displayVis=true;scr->height
    mode = NORMAL;
    addMods();
    //ScreenObjects.push_back(new mainSettingsButton());
    //ScreenObjects[ScreenObjects.size() - 1]->pos = mPoint(settingsButton::size, settingsButton::size);
    
    
    
    //gameRowCount=2;

    //Add info box for main menu
    //ScreenObjects.push_back(new gameInfoObj(scr->height / 3, scr->width / 2 + 100, mPoint(scr->width / 2,scr->height / 3 / 2
     //                                       /*scr->height / 3 / 2 + scr->height / 3 / 4*/      )));
    
//    dynamic_cast<gameInfoObj*>(ScreenObjects[ScreenObjects.size() - 1])->title = "No game";
//    dynamic_cast<gameInfoObj*>(ScreenObjects[ScreenObjects.size() - 1])->gameIdHexString = "Unknown";
//    gameSelectMenu::addVisibleGames();
    //todo add empty gamecard +
}





settingMenu::settingMenu(menu* oldMenuInput): menu(oldMenuInput) {}


mainSettingMenu::mainSettingMenu(menu* oldMenuInput): settingMenu(oldMenuInput) {
    curPos = mPoint(0, 0);
    //    curPos = mPoint(scr->width/2,scr->height/2);

    ScreenObjects.push_back(new settingItem(scr->width));
    u32 halfWid  =  dynamic_cast<settingItem*>(ScreenObjects[ScreenObjects.size() - 1])->width / 2;
    u32 halfHei  =  dynamic_cast<settingItem*>(ScreenObjects[ScreenObjects.size() - 1])->height / 2;
    std::stringstream ss;
    ss << "H: " << halfHei << "\nW:" << halfWid;

    scr->drawText(ss.str(), mPoint(80, 80), (*fontMap)["sans65"]/* fontMap->find("symbola_65")->second */, halfWid * 2, false,
                  RGBA8_MAXALPHA(0, 0, 0));
    ScreenObjects[ScreenObjects.size() - 1]->pos = mPoint(curPos.x + halfWid, curPos.y + halfHei);

    //dynamic_cast<settingItem*>(ScreenObjects[ScreenObjects.size() - 1])->jsonObj.pushBack("Test key: ",new jsonDataTypeString("val"));

}

inputMenu::inputMenu(menu* oldMenuInput): menu(oldMenuInput) {
    fontPadding = 8;
    radius = 40;
    drawBGBool = false;
    boxColor = RGBA8_MAXALPHA(128, 120, 0);
    ///    menuWid = xxx;
    ///    menuHei = xxx;
    menuWid = scr->width / 4;
    menuHei = scr->height / 4;
}
void inputMenu::setOptions(std::vector<std::tuple<std::string, std::string, eventContainer*>>& inputOptions) {
    buttonOptions.insert(buttonOptions.end(), inputOptions.begin(), inputOptions.end());
    turnOptionsIntoScreenObjects();
}
void inputMenu::turnOptionsIntoScreenObjects() {
    //clear all old objects
    for(size_t i = 0; i < ScreenObjects.size(); ++i) {
        delete ScreenObjects[i];
    }

    //    curPos

    int heightPos = curPos.y;
    int lineCount = 1; //will need a function to fix this later;
    // if(buttonOptions.size()>0){
    //    lineCount = 1; //also will need to be changed std::get<0>(buttonOptions[0]);
    //    heightPos+= (fontMap->find(std::get<1>(buttonOptions[0]))->second->charHeight* lineCount)/2;
    //}



    //Trying out something new(lambdas)
    //Not sure if worth on any level
    //Also not sure if I should limit the text to only being so wide, or if I should make it just expand to fit the longest line
    //Nothing good will come from this
    //It almost puts ternary operators to shame: auto would be a good replacement if I used c++14
    auto maxStrLenIt = std::max_element(buttonOptions.begin(),
                                        buttonOptions.end(), [&](const std::tuple<std::string, std::string, eventContainer*>& t1,
                                                const std::tuple<std::string, std::string, eventContainer*>& t2)->bool{return fontMap->at(std::get<1>(t1))->getTextLength(std::get<0>(t1)) < fontMap->at(std::get<1>(t2))->getTextLength(std::get<0>(t2));});

    menuWid = fontMap->at(std::get<1>(*maxStrLenIt))->getTextLength(std::get<0>(*maxStrLenIt)) + fontPadding ;

    for(size_t i = 0; i < buttonOptions.size(); ++i) {
        //if(){
        //heightPos+= (fontMap->find(std::get<1>(buttonOptions[i]))->second->charHeight* lineCount)/2;
        //}

        //fontMap->find(lastSimpleOption->fontName)->second->charHeight* lineCount + fontPadding;

        lineCount = 1;
        ScreenObjects.push_back(new simpleOption());

        simpleOption* lastSimpleOption = dynamic_cast<simpleOption*>(ScreenObjects[ScreenObjects.size() - 1]);
        lastSimpleOption->height = (fontMap->find(std::get<1>(buttonOptions[i]))->second->charHeight * lineCount);
        heightPos += lastSimpleOption->height / 2;
        lastSimpleOption->pos =        mPoint(curPos.x, heightPos);
        lastSimpleOption->text =          std::get<0>(buttonOptions[i]);
        lastSimpleOption->fontName =      std::get<1>(buttonOptions[i]);
        lastSimpleOption->eventFunctor =  std::get<2>(buttonOptions[i]);
        lastSimpleOption->width = menuWid;
        heightPos += lastSimpleOption->height / 2;
    }

    menuHei = heightPos;
}
void inputMenu::draw() {
    scr->drawRoundedRect(mPoint(curPos.x - (menuWid / 2 + fontPadding), curPos.y), mPoint(curPos.x + menuWid / 2 + fontPadding,
                         curPos.y + menuHei), radius, boxColor);
    menu::draw();
}
void inputMenu::logic() {

    if((mainController->kDown & KEY_Y) || (mainController->kHeld & KEY_Y)) {
        scr->background(bgColor);
    }

    if(ScreenObjects.size() > 0) {
        ScreenObjects[hoveredId]->isSelected = false;

        if(std::abs(mainController->posLeftStick.dx) || std::abs(mainController->posLeftStick.dy)) {
            ScreenObjects[hoveredId]->isHovered = false;
            hoveredId = mainController->getClosestPoint(ScreenObjects[hoveredId]->pos, ScreenObjects, hoveredId, scr);
            ScreenObjects[hoveredId]->isHovered = true;
        }

        if(mainController->touchCount > 0) {
            scr->drawCircle(mPoint(mainController->touch.px, mainController->touch.py), 10, RGBA8_MAXALPHA(255, 0, 0));
            ScreenObjects[hoveredId]->isHovered = false;
            //std::sqrt(this->x * this->x + this->y * this->y);
            float smallLen =   mVec(mainController->touch.px - ScreenObjects[hoveredId]->pos.x,
                                    mainController->touch.py - ScreenObjects[hoveredId]->pos.y).length();

            for(size_t i = 0; i < ScreenObjects.size(); i++) {
                float tmpLen = mVec(mainController->touch.px - float(ScreenObjects[i]->pos.x),
                                    mainController->touch.py - float(ScreenObjects[i]->pos.y)).length();

                if(ScreenObjects[i]->canHover && abs(tmpLen) < abs(smallLen)) { //ScreenObjects[hoveredId]->pos
                    smallLen = tmpLen;
                    hoveredId = i;
                }
            }

            ScreenObjects[hoveredId]->isHovered = true;
            //controller sets key a to down
        }

        if((mainController->kHeld & KEY_A) || (mainController->kDown & KEY_A) || mainController->touchCount) {
            ScreenObjects[hoveredId]->isSelected = true;
        }

        ScreenObjects[hoveredId]->logic(this);
    }



    //if((mainController->kDown & KEY_B) || (mainController->kHeld & KEY_B)) {
    //    *(this->activeMenuPtr) = this->oldMenuPtr;
    //    delete this;
   // }


}
/*void gameSelectMenu::addGameCard(gameCard* gC) {
    int gap = gameCard::width / 2 / 4;
    int wid = gameCard::width;
    gC->listPosId = gameCardCount;
    gameCardCount++;
    ScreenObjects.push_back(gC);

    if(gameCardCount == 1) {//not sure why i did this
        ScreenObjects[ScreenObjects.size() - 1]->pos = mPoint((gameCardCount) * (wid) - wid / 2 + (gameCardCount) * gap,
                scr->height - (gameCard::height / 2) - 2 * gap);
    }
    else {
        mPoint lastPoint = ScreenObjects[ScreenObjects.size() - 2]->pos;
        lastPoint.x += gap + wid;
        ScreenObjects[ScreenObjects.size() - 1]->pos = lastPoint;
    }
}
 * */




void gameSelectMenu::logic() {
    //maybe add clock here? do I make these else ifs?
    if(ScreenObjects.size() > 0) {
        switch (mode){
        case NORMAL: 
            hoverCorrectScreenObjFromJoyConIfNeeded();
            hoverCorrectScreenObjFromTouchIfNeeded();
            ScreenObjects[hoveredId]->logic(this);
            if(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])!=nullptr && dynamic_cast<gameInfoObj*>(ScreenObjects[1]) != nullptr){
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->title = dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->title;
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->gameIdHexString = convertToHexString(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId);//dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->title;
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->isFsModsEnabled = gSettings::getInstance().main->getModStatus(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId,settings::fsModsKey);
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->isExeFsModsEnabled = gSettings::getInstance().main->getModStatus(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId,settings::exeModsKey);
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->fsModCount = gSettings::getInstance().main->getModCount(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId,settings::fsModsKey);
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->exefsModCount = gSettings::getInstance().main->getModCount(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId,settings::exeModsKey);
                
            }
            break;
        case MOVING_GC: 
        //todo make touch a function of controller and add that here
            if(std::abs(mainController->posLeftStick.dx) || std::abs(mainController->posLeftStick.dy)) {

                    unsigned int newHoveredId = mainController->getClosestPoint(ScreenObjects[hoveredId]->pos, ScreenObjects, hoveredId, scr);
                    if(newHoveredId!=hoveredId &&
                        dynamic_cast<gameCard*>(ScreenObjects[newHoveredId]) != nullptr &&
                        dynamic_cast<gameCard*>(ScreenObjects[hoveredId]) != nullptr && 
                        !(dynamic_cast<gameCard*>(ScreenObjects[newHoveredId])->isToggleGamecardIcon || dynamic_cast<gameCard*>(ScreenObjects[newHoveredId])->isToggleGamecardIcon)
                        ){
                        swapGCs(dynamic_cast<gameCard*>(ScreenObjects[newHoveredId]), dynamic_cast<gameCard*>(ScreenObjects[hoveredId]));
                        std::swap(ScreenObjects[hoveredId],ScreenObjects[newHoveredId]);
                        //Idk why I swap their positions in screenObjects
                        hoveredId = newHoveredId;
                    }
            }
            if((mainController->kDown & KEY_B) || (mainController->kHeld & KEY_B)) {
                mode = NORMAL;
            }
            break;

        case DEBUG: 
            if((mainController->kDown & KEY_Y) || (mainController->kHeld & KEY_Y)) {
                scr->background(bgColor);
            }
            ScreenObjects[hoveredId]->logic(this);
            break;

        }


    }

}
void modSelectMenu::logic() {
    //maybe add clock here? do I make these else ifs?
    //scr->drawRect(mPoint(0, 0), mPoint(300, 300), RGBA8_MAXALPHA(0xcc,0xcc,0xcc));
    
    if((mainController->kDown & KEY_B) || (mainController->kHeld & KEY_B)) {
        deleteMenuAndGoToPrevMenu();
    }
    

    if(ScreenObjects.size() > 0) {
        /*
        switch (mode){
        case NORMAL: 
            hoverCorrectScreenObjFromJoyConIfNeeded();
            hoverCorrectScreenObjFromTouchIfNeeded();
            ScreenObjects[hoveredId]->logic(this);
            if(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])!=nullptr && dynamic_cast<gameInfoObj*>(ScreenObjects[1]) != nullptr){
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->title = dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->title;
                dynamic_cast<gameInfoObj*>(ScreenObjects[1])->gameIdHexString = convertToHexString(dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->gameTitleId);//dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->title;
            }
            break;
        case MOVING_GC: 
        //todo make touch a function of controller and add that here
            if(std::abs(mainController->posLeftStick.dx) || std::abs(mainController->posLeftStick.dy)) {

                    unsigned int newHoveredId = mainController->getClosestPoint(ScreenObjects[hoveredId]->pos, ScreenObjects, hoveredId, scr);
                    if(newHoveredId!=hoveredId &&
                        dynamic_cast<gameCard*>(ScreenObjects[newHoveredId]) != nullptr &&
                        dynamic_cast<gameCard*>(ScreenObjects[hoveredId]) != nullptr && 
                        !(dynamic_cast<gameCard*>(ScreenObjects[newHoveredId])->isToggleGamecardIcon || dynamic_cast<gameCard*>(ScreenObjects[newHoveredId])->isToggleGamecardIcon)
                        ){
                        swapGCs(dynamic_cast<gameCard*>(ScreenObjects[newHoveredId]), dynamic_cast<gameCard*>(ScreenObjects[hoveredId]));
                        std::swap(ScreenObjects[hoveredId],ScreenObjects[newHoveredId]);
                        //Idk why I swap their positions in screenObjects
                        hoveredId = newHoveredId;
                    }
            }
            if((mainController->kDown & KEY_B) || (mainController->kHeld & KEY_B)) {
                mode = NORMAL;
            }
            break;

        case DEBUG: 
            if((mainController->kDown & KEY_Y) || (mainController->kHeld & KEY_Y)) {
                scr->background(bgColor);
            }
            ScreenObjects[hoveredId]->logic(this);
            break;

        }
        */

    }

}


void gameSelectMenu::swapGCs(gameCard *oGC,gameCard * nGC){
    gSettings::getInstance().main->swapGameListPosition(oGC->settingsFileArrayPosId,nGC->settingsFileArrayPosId);
    //unsigned int tmpListPosId = oGC->listPosId;
    mPoint tmpPos = oGC->pos;
    oGC->pos = nGC->pos;
    nGC->pos = tmpPos;
    
    //oGC->listPosId = nGC->listPosId;
    //nGC->listPosId = tmpListPosId;
}
gameSelectMenu::~gameSelectMenu() {
    //    gameSelectMenu::~menu();
}


void mainSettingMenu::logic() {

    if(std::abs(mainController->posLeftStick.dx) || std::abs(mainController->posLeftStick.dy)) {
        ScreenObjects[hoveredId]->isHovered = false;
        hoveredId = mainController->getClosestPoint(ScreenObjects[hoveredId]->pos, ScreenObjects, hoveredId, scr);
        ScreenObjects[hoveredId]->isHovered = true;
    }



    if((mainController->kDown & KEY_Y) || (mainController->kHeld & KEY_Y)) {
        scr->background(bgColor);
    }


    //make sure this is the last if or make it the first and the reset else if
    if((mainController->kDown & KEY_B) || (mainController->kHeld & KEY_B)) {
        *(this->activeMenuPtr) = this->oldMenuPtr;
        delete this;
    }

}


    //old stuff. Very garbage
    
    /*
    if(dynamic_cast<gameCard*>(ScreenObjects[hoveredId]) != nullptr) {
        unsigned int hoveredListPosId = dynamic_cast<gameCard*>(ScreenObjects[hoveredId])->listPosId;
        unsigned int rightGameCardScreenObjId = 0 ;
        unsigned int leftGameCardScreenObjId = 0;
        //Move other cards left
//        if( (hoveredListPosId+1) != gameCardCount){
            for(size_t i=0;i<ScreenObjects.size();++i){
                gameCard * tmpScrObjId = dynamic_cast<gameCard*>(ScreenObjects[i]);
                if(tmpScrObjId != nullptr){
                    if(tmpScrObjId->listPosId ==  (hoveredListPosId - 1 ) ){leftGameCardScreenObjId = i;}
                    else if(tmpScrObjId->listPosId ==  (hoveredListPosId + 1 )  ){rightGameCardScreenObjId = i;}
                    
                    if(tmpScrObjId->listPosId > hoveredListPosId){
                        
                        //it is a gamecard that is to the left of the current hovered id
                        tmpScrObjId->listPosId -=1;
                        tmpScrObjId->pos.x -= (gameCard::width + gameCard::width/2/4);
                        
                    }
                }
            }
//        }
        //set hovered it to "closest" card

        //Delete card
        
        ScreenObjects.erase(ScreenObjects.begin()+hoveredId);
        //hoveredId = maxGameCardId;
        
        if((hoveredListPosId+1) != gameCardCount){
            if(hoveredId < rightGameCardScreenObjId){rightGameCardScreenObjId--;}
            hoveredId = rightGameCardScreenObjId;
        }
        else{
            if(hoveredId < leftGameCardScreenObjId){leftGameCardScreenObjId--;}
            hoveredId = leftGameCardScreenObjId;
        }

        ScreenObjects[hoveredId]->isHovered = true;
        gameCardCount--;
    }
    */
