#include "shapes.h"

     u32 modItem::borderColor =RGBA8_MAXALPHA(65, 65, 70);
     const u32 modItem::hoveredBorderColor = RGBA8_MAXALPHA(100,20,200);
     const u32 modItem::enabledBorderColor = RGBA8_MAXALPHA(255, 158, 23);
     const u32 modItem::height = 128;
     const u32 modItem::width = 128;

     const u32 modItem::innerRectColor = RGBA8_MAXALPHA(255, 255, 255);
    
     const u32 modItem::borderThick = 5;
     
     





scrObject::scrObject():
    canSelect(false),
    isVisible(true),
    isSelected(false),
    isHovered(false),
    canHover(false),
    hasLogic(false) {
}


scrObject::scrObject(mPoint inPt): scrObject() {
    pos = inPt;
}


polygonShape::polygonShape(mPoint inputPoints[], unsigned int inPointCount): scrObject() {
    pointCount = inPointCount;
    points = new mPoint[pointCount];

    for(unsigned int i = 0; i < pointCount; ++i) {
        points[i] = inputPoints[i];
    }

    color = RGBA8_MAXALPHA(255, 255, 255);
}
polygonShape::~polygonShape() {
    delete [] points;
}


u32 gameCard::borderColor = RGBA8_MAXALPHA(65, 65, 70); //RGBA8_MAXALPHA(78, 79, 85);
const u32 gameCard::innerRectColor = RGBA8_MAXALPHA(0, 0, 0);
const u32 gameCard::innerRectTopColor = RGBA8_MAXALPHA(255, 34, 12);
const u32 gameCard::height = 9*23;//34 * 9 ; // 8 is the scale factor
const u32 gameCard::width = height ;//* 23 / 34;
const u32 gameCard::sideBorderThick = 6;
const u32 gameCard::topBorderThick = sideBorderThick * 1.3 + 2;
const u32 gameCard::bottomBorderThick = 2 * topBorderThick;

gameCard::gameCard(u64 gameTitleID): scrObject() {
    title="";
    isToggleGamecardIcon = false;
    gameTitleId = gameTitleID;
    canSelect = true;
    canHover = true;
    //isVisible = false;
    triColor = RGBA8_MAXALPHA(0, 0, 0);
    hoveredTriColor = RGBA8_MAXALPHA(163, 156, 239);

//    gameTitleId=0;
    listPosId = 0;
    
    settingsFileArrayPosId=0;
    std::string filePath = "romfs:/missing.jpg";
    std::ifstream imgFile(filePath.c_str(), std::ios::binary | std::ios::ate);
    std::streamsize imgFileSize = imgFile.tellg();
    imgFile.seekg(0, std::ios::beg);

    char* imgFileBuff = new char [imgFileSize];
    imgFile.read(imgFileBuff, imgFileSize);
    imgFile.close();
    iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(imgFileBuff), imgFileSize);
    delete [] imgFileBuff;
    if(gameTitleID!=0){
        gameCard::updateIcon();
    }

}
int gameCard::updateIcon(menu* mnu){
    std::string filePath = "romfs:/missing.jpg";
    if(dynamic_cast<gameSelectMenu*>(mnu)!=nullptr && dynamic_cast<gameSelectMenu*>(mnu)->displayVis){
        filePath = "romfs:/plus.jpg";
    }
    else{
        filePath = "romfs:/return.jpg";
    }
    std::ifstream imgFile(filePath.c_str(), std::ios::binary | std::ios::ate);
    std::streamsize imgFileSize = imgFile.tellg();
    imgFile.seekg(0, std::ios::beg);

    char* imgFileBuff = new char [imgFileSize];
    imgFile.read(imgFileBuff, imgFileSize);
    imgFile.close();
    delete iconImg;
    iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(imgFileBuff), imgFileSize);
    delete [] imgFileBuff;
    return 0;
}



int gameCard::updateIcon(){
    
    size_t bufoutSz = 0x20000;//This is a rough estimate, for binding of issac it would be 0x18862
    size_t cpSz = 0;
    char* bufoutnew = new char[bufoutSz];
    Result importJpegRes = getTitleData(gameTitleId, bufoutnew, bufoutSz, cpSz, title);

    if(!importJpegRes) {
            delete iconImg;
            iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(bufoutnew), cpSz);
            removeNonAsciiChars(title);
    }

    delete [] bufoutnew;
    return 0;
}
/*
    std::vector<u64>tidList;
    size_t readtidSize = 0;
    getTitleList(FsStorageId_GameCard, tidList, readtidSize);
    
    title = "";
    gameTitleId = 0;

    if(readtidSize > 0) {

        gameTitleId = tidList[0];
        size_t cpSz = 0;
        size_t bufoutSz = 0x20000;//This is a rough estimate, for binding of issac it would be 0x18862
        char* bufoutnew = new char[bufoutSz];
        //Result betterImportJpeg(u64 titleID,char* bufout,size_t bufSz,size_t & copiedSize)
        Result importJpegRes = getTitleData(tidList[0], bufoutnew, bufoutSz, cpSz, title);
        //importJpeg(FsStorageId_GameCard, tidList[0], bufoutnew, bufoutSz);
        gameTitleId = cpSz;// getTitleData(u64 titleID, char* bufout, size_t bufSz, size_t& copiedSize,std::string &titleName)

        if(!importJpegRes) {
            iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(bufoutnew), cpSz);
            removeNonAsciiChars(title);
        }
        else {//error fallback to default
            iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(imgFileBuff), imgFileSize);
        }

        delete [] bufoutnew;
    }
    else {
        iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(imgFileBuff), imgFileSize);
    }
*/
modItem::~modItem(){
    delete iconImg;
}
gameCard::~gameCard() {
    delete iconImg;
}
gameCard::gameCard(u64 gameTitleID,mPoint position): gameCard(gameTitleID) {
    pos = position;
}

void gameCard::logic(menu* mnu) {
    if((mnu->mainController->kDown & KEY_Y) /*|| (m_menu->mainController->kHeld & KEY_A)*/) {
        dynamic_cast<gameSelectMenu*>(mnu)->addGameCard(new gameCard(gameTitleId));
        
        
    }
    else if(mnu->mainController->kDown & KEY_A){
        if(!isToggleGamecardIcon){
            menu * setMenu  = new inputMenu(mnu);
            //
            std::vector<std::tuple<std::string, std::string, eventContainer*>> inputOptions{
                {"Edit LayeredFS Mods", "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::createFsModMenu))},
                {"Edit ExeFs Mods"    , "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::createExeFsModMenu))},
                {"Move Game"          , "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::enableGameMoving))},
                {"Hide Game"          , "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::toggleHoveredGameCardScreen))},//deleteHoveredGameCard
                {"AnotherEvent"       , "sans24", new menuEvent(mnu, &menu::dummy)}
            };
            dynamic_cast<inputMenu*>(setMenu)->setOptions(inputOptions);
        }
        else{
            dynamic_cast<gameSelectMenu*>(mnu)->toggleVisibilityMode();
        }
        
    }

    //I can see some cool optimization going on here
    //todo
    //another option would be to move it left by a percentage of how much it needs to move. This would make it very fast  if 50% was used
    //issue may be infinityl small and alwasy me boving to get the last little part // not really an issue 
    
    
    int leftLimit =   //int(gameCard::width / 2) + int(gameCard::width / 2 / 4) + int(gameCard::width) + int(gameCard::width / 2 / 4);
                    int(2*gameCard::width/3 +  gameSelectMenu::gapBetweenGameCards);
    int rightLimit = //int(mnu->scr->width) - int(gameCard::width / 2) - int(gameCard::width / 2 / 4) - int(gameCard::width) - int(
                     //            gameCard::width / 2 / 4);
                    int(mnu->scr->width - 2*gameCard::width/3 - 2*gameSelectMenu::gapBetweenGameCards);
    if(pos.x < leftLimit) {
        //another check to see if it is the first gamecard     gC.listPosId = gameCardCount
        if(this->listPosId == 0 && pos.x < int(gameCard::width / 2) + int(gameCard::width / 2 / 4)) {
            dynamic_cast<gameSelectMenu*>(mnu)->moveGCDS((int(gameCard::width / 2) + int(gameCard::width / 2 / 4) - pos.x) / 2);
        }
        else if(//this != mnu->ScreenObjects[mnu->ScreenObjects.size() - dynamic_cast<gameSelectMenu*>(mnu)->gameCardCount]   ||
                    pos.x < int(gameCard::width / 2) + int(gameCard::width) + int(gameCard::width / 2 / 4)) {
            dynamic_cast<gameSelectMenu*>(mnu)->moveGCDS((leftLimit - pos.x) / (4));
        }
    }
    else if(pos.x > rightLimit) {
        dynamic_cast<gameSelectMenu*>(mnu)->moveGCDS(-(pos.x - rightLimit) / (4));
    }





}
void gameCard::draw(menu* mnu) {

    mPoint offset(0, 0);

    if(this->isVisible) {
        //        scr->drawRect(mPoint(pos.x, pos.y), mPoint(pos.x + iconWidth, pos.y + iconHeight), iconBgColor);
            u32 tmpColor = gameCard::borderColor;// = RGBA8_MAXALPHA(65, 65, 70); 

        if(this->isHovered) {
            borderColor = RGBA8_MAXALPHA(100,20,200);
            //offset.y = -20;
        }

        //Draw first rectangle
        mnu->scr->drawRect(mPoint(pos.x - (width / 2) + offset.x, pos.y - (height / 2) + offset.y), mPoint(pos.x + (width / 2) + offset.x,
                           pos.y + (height / 2) + offset.y), borderColor);

        //Draw corner rounds
        mnu->scr->drawCircle(mPoint(pos.x - (width / 2) + sideBorderThick + offset.x, pos.y - (height / 2) + offset.y), sideBorderThick,
                             borderColor);
        mnu->scr->drawCircle(mPoint(pos.x + (width / 2) - sideBorderThick + offset.x, pos.y - (height / 2) + offset.y), sideBorderThick,
                             borderColor);
        mnu->scr->drawCircle(mPoint(pos.x + (width / 2) - sideBorderThick + offset.x, pos.y + (height / 2) + offset.y), sideBorderThick,
                             borderColor);
        mnu->scr->drawCircle(mPoint(pos.x - (width / 2) + sideBorderThick + offset.x, pos.y + (height / 2) + offset.y), sideBorderThick,
                             borderColor);

        //Fill top and bottom gaps
        mnu->scr->drawRect(mPoint(pos.x - (width / 2) + sideBorderThick + offset.x, pos.y - (height / 2) - sideBorderThick + offset.y),
                           mPoint(pos.x + (width / 2) - sideBorderThick + offset.x, pos.y - (height / 2) + offset.y), borderColor);
        mnu->scr->drawRect(mPoint(pos.x + (width / 2) - sideBorderThick + offset.x, pos.y + (height / 2) + sideBorderThick + offset.y),
                           mPoint(pos.x - (width / 2) + sideBorderThick + offset.x, pos.y + (height / 2) + offset.y), borderColor);

        //draw inner rect/Not needed now
        //mnu->scr->drawRect(mPoint(pos.x - (width / 2) + sideBorderThick + offset.x, pos.y - (height / 2) + topBorderThick + offset.y),
        //                   mPoint(pos.x + (width / 2) - sideBorderThick + offset.x, pos.y + (height / 2) - bottomBorderThick + offset.y), innerRectColor);

        //Draw small inner black border
        int innerblackBorderSz = 3;
        mnu->scr->drawRectBorder(mPoint(pos.x - (width / 2) + sideBorderThick + offset.x - innerblackBorderSz,
                                        pos.y - (height / 2) + topBorderThick + offset.y - innerblackBorderSz),
                                 mPoint(pos.x + (width / 2) - sideBorderThick + offset.x + innerblackBorderSz,
                                        pos.y + (height / 2) - bottomBorderThick + offset.y + innerblackBorderSz), innerblackBorderSz, innerRectColor);

        //Draw weird rect at top of card
        mnu->scr->drawRect(mPoint(pos.x - (width / 6), pos.y - (height / 2) + offset.y),
                           mPoint(pos.x + (width / 6),  pos.y - (height / 2) + topBorderThick / 3 + offset.y), innerRectColor);

        //drawRectBorder(mPoint pt1, mPoint pt2, u32 borderSize, u32 color);
        //draw triangle+offset.y
        mnu->scr->drawTriangle(mPoint(pos.x - bottomBorderThick / 2 + offset.x, pos.y + (height / 2) - sideBorderThick + offset.y),
                               mPoint(pos.x + bottomBorderThick / 2 + offset.x, pos.y + (height / 2) - sideBorderThick + offset.y),
                               mPoint(pos.x + offset.x, pos.y + (height / 2) + offset.y),
                               (this->isHovered) ? hoveredTriColor : triColor);

        //drawtid
        std::ostringstream tidSS;
        //this is dirty
        unsigned int dispValUUe=0;
        for(unsigned int i=0;i<mnu->ScreenObjects.size();i++){
            if(dynamic_cast<gameCard*>(mnu->ScreenObjects[i])==this){
                dispValUUe=i;
            }
        }
        
        tidSS << dispValUUe;//std::hex << <<//gameTitleId ;

        //Draw image
        if(iconImg->GetResult() == Jpeg::Decoder::OK && iconImg->IsColor()) {
            mnu->scr->drawRGB8(mPoint(pos.x - (width / 2) + sideBorderThick + offset.x, pos.y - (height / 2) + topBorderThick + offset.y),
                               iconImg->GetImage(), iconImg->GetWidth(), iconImg->GetHeight(),  width - 2 * sideBorderThick/*iconImg->GetWidth()*/,
                               height - bottomBorderThick - topBorderThick/*iconImg->GetHeight()*/);
        }

        mnu->scr->drawText(tidSS.str(), pos,  mnu->fontMap->find("sans14")->second, mnu->scr->width, true, RGBA8_MAXALPHA(255, 0, 0));
        mnu->scr->drawText(title, mPoint(pos.x, pos.y + 40),  mnu->fontMap->find("sans14")->second, mnu->scr->width, true,
                           RGBA8_MAXALPHA(255, 0, 0));
        if(this->isHovered) {
            borderColor = tmpColor;
        }

    }
}
//simpleOption
simpleOption::simpleOption(): scrObject(), borderColor(RGBA8_MAXALPHA(0, 0, 0)), innerColor(RGBA8(0, 255, 0, 255)) {
    canSelect = true;
    canHover = true;
    lineColor = RGBA8_MAXALPHA(0, 128, 0);
    hoveredTextColor = RGBA8_MAXALPHA(82, 76, 255);
    clickedTextColor = RGBA8_MAXALPHA(80, 30, 200);
    defaultTextColor = RGBA8_MAXALPHA(0, 0, 0);
    lineHeight = 4;
}
simpleOption::simpleOption(mPoint position): simpleOption() {
    pos = position;
}
void simpleOption::draw(menu* mnu) {
    //add a rect latter
    if(this->isVisible) {
        int linecount = 1;
        u32 fontColor = (isSelected) ? clickedTextColor : (isHovered) ? hoveredTextColor : defaultTextColor;
        mnu->scr->drawText(text, mPoint(pos.x, pos.y - (linecount * mnu->fontMap->find(fontName)->second->charHeight / 2)),
                           mnu->fontMap->find(fontName)->second, width, true, fontColor);
        inputMenu* inMnuPtr = dynamic_cast<inputMenu*>(mnu);

        if(inMnuPtr != nullptr) { //This is a bit dirty and wont look good if the radius/text is too small
            if((pos.y + height / 2) < (inMnuPtr->menuHei - inMnuPtr->radius))
            { mnu->scr->drawRect(mPoint(pos.x - width / 2, pos.y + height / 2), mPoint(pos.x + width / 2, pos.y + height / 2 + lineHeight), lineColor); }
        }
    }
}
void simpleOption::logic(menu* mnu) {
    if((mnu->mainController->kDown & KEY_A) /*|| (m_menu->mainController->kHeld & KEY_A)*/) {
        
        //mnu->deleteMenuAndGoToPrevMenu();//well this is a problem should this go before or after? if after it deletes a new menu right after creating it
        //if before then the new menu 
        
        //Im gonna try only deleting the menu from the function that is called
        eventFunctor->trigger();
        //mnu->mainController->specialEvent |= controlEventCode;
    }
    else if((mnu->mainController->kDown & KEY_B) /*|| (m_menu->mainController->kHeld & KEY_A)*/) {
        //eventFunctor->trigger();
        mnu->deleteMenuAndGoToPrevMenu();
    }
}


gameInfoObj::gameInfoObj(u32 hei, u32 wid): scrObject(), borderColor(RGBA8_MAXALPHA(0, 0, 0)), innerColor(RGBA8(0, 255, 0, 80)),
    height(hei), width(wid) {
    canSelect = false;
    canHover = false;
    isFsModsEnabled=true;
    isExeFsModsEnabled=true;
    fsModCount=0;
    exefsModCount=0;
}
gameInfoObj::gameInfoObj(u32 hei, u32 wid, mPoint position): gameInfoObj(hei, wid) {
    pos = position;
}


void gameInfoObj::draw(menu* mnu) {

    if(this->isVisible) {
        //sets offset to upper left corrner
        mPoint offset(pos.x - width / 2, pos.y - height / 2);

        u32 borderSize = 4;

        mnu->scr->drawRoundedRect(mPoint(offset.x + borderSize, offset.y + borderSize), mPoint(offset.x + width - borderSize,
                                  offset.y + height + 1 - borderSize), 40, innerColor);

        u32 padding = 13;
        mnu->scr->drawText(title, mPoint(offset.x + padding, offset.y + padding), mnu->fontMap->find("sans24")->second, width, false, RGBA8_MAXALPHA(0, 0, 0));
        int lastHeight = getTextHeight(title, mnu->fontMap->find("sans24")->second, width,false);// bool isCentered = false);
//        std::vector<std::string>stringVec = {"Game Hex Id: " + gameIdHexString, ""};

  //      for(int i=0;i<stringVec.size();i++){
          std::string nextText = string("Game Hex Id: ") + gameIdHexString;
          mnu->scr->drawText(nextText, mPoint(offset.x + padding, offset.y + padding + lastHeight ), mnu->fontMap->find("sans14")->second, width, false, RGBA8_MAXALPHA(0, 0, 0));
          lastHeight += getTextHeight(nextText, mnu->fontMap->find("sans14")->second, width,false);
          
           nextText="Fs Mod Status: " + string((isFsModsEnabled)?"Enabled":"Disabled");
          mnu->scr->drawText(nextText, mPoint(offset.x + padding, offset.y + padding + lastHeight ), mnu->fontMap->find("sans14")->second, width, false, ((isFsModsEnabled)?RGBA8_MAXALPHA(80, 255, 100):RGBA8_MAXALPHA(220, 60, 60)));
          std::string lastText = nextText;
          nextText = "Mod Count: " + std::to_string(fsModCount);
          mnu->scr->drawText(nextText, mPoint(offset.x + padding + mnu->fontMap->find("sans14")->second->getTextLength(lastText), offset.y + padding + lastHeight ), mnu->fontMap->find("sans14")->second, width, false, RGBA8_MAXALPHA(0, 0, 0));
          lastHeight += getTextHeight(nextText+lastText, mnu->fontMap->find("sans14")->second, width,false);
          
          nextText="exeFs Mod Status: " + string((isExeFsModsEnabled)?"Enabled":"Disabled");
          mnu->scr->drawText(nextText, mPoint(offset.x + padding, offset.y + padding + lastHeight ), mnu->fontMap->find("sans14")->second, width, false, ((isExeFsModsEnabled)?RGBA8_MAXALPHA(80, 255, 100):RGBA8_MAXALPHA(220, 60, 60)));
          lastText = nextText;
          nextText = "Mod Count: " + std::to_string(exefsModCount);
          mnu->scr->drawText(nextText, mPoint(offset.x + padding + mnu->fontMap->find("sans14")->second->getTextLength(lastText), offset.y + padding + lastHeight ), mnu->fontMap->find("sans14")->second, width, false, RGBA8_MAXALPHA(0, 0, 0));
          lastHeight += getTextHeight(nextText+lastText, mnu->fontMap->find("sans14")->second, width,false);

          
    //    }


        //        mnu->scr->drawRectBorder(offset, mPoint(offset.x + width, offset.y + height), borderSize, borderColor);
        //        mnu->scr->drawRect(mPoint(offset.x + borderSize, offset.y + borderSize), mPoint(offset.x + width - borderSize,
        //                       offset.y + height + 1 - borderSize), innerColor);
        //        mnu->scr->drawRect(mPoint(offset.x+borderSize-80,offset.y + borderSize), mPoint((offset.x + width - borderSize)/2, (offset.y + height + 1 - borderSize)/2), RGBA8(255,0,0));

        //u32 lastTextHeight = mnu->fontMap->find("sans48")->second->charHeight;
        //maybe add multiplier for if the title is more than one line long
        //alt way is a bit hard to understand (*(mnu->fontMap))["symbola_65"]
        //the alt way would kinda work better if I set a default constructor for fonts, because then it "should" create it on the spot if it was not found
        //should I make these defined globaly? #define A_BUTTON_INVERSE = "\xF0\x9F\x85\x90"
        
        /*
        std::string buttons = u8"\xF0\x9F\x85\x90\n\xF0\x9F\x85\x91\n\xF0\x9F\x85\xA7\n\xF0\x9F\x85\xA8"; //Inverse Colors
        //  u8"Ⓐ\nⒷ\nⓍ\nⓎ\n";//default
        //  u8"\xF0\x9F\x85\xB0\n\xF0\x9F\x85\xB1\n\xF0\x9F\x86\x87\n\xF0\x9F\x86\x88";//inverse rounded square
        std::string instruct =   "Enter Mod Menu"
                                 "\nSome setting"
                                 "\nToggle Moving"
                                 "\nRemove from list";
        mnu->scr->drawText(buttons, mPoint(pos.x - mnu->fontMap->find("sans24")->second->getTextLength(instruct) / 2, offset.y + padding),
                           mnu->fontMap->find("symbols24")->second, width, false, RGBA8_MAXALPHA(255, 0, 255));
        mnu->scr->drawText(instruct, mPoint(pos.x + mnu->fontMap->find("symbols24")->second->getTextLength(u8"Ⓐ"), offset.y + padding),
                           mnu->fontMap->find("sans24")->second, width, true, RGBA8_MAXALPHA(255, 0, 0));
        */

        //        std::ostringstream ss;
        //        size_t pos = 0;
        //        ss << std::hex << getCodePoint(buttons, pos, pos) ;
//        std::ostringstream ss;
//        ss << mnu->fontMap->find("sans24")->second->getTextLength("A");

        //mnu->scr->drawCircle(pos,40,RGBA8_MAXALPHA(255,255,112));
        

        //mnu->scr->drawText(ss.str(), mPoint(pos.x, offset.y + padding), mnu->fontMap->find("sans24")->second, width, true, RGBA8_MAXALPHA(12, 120, 255));


    }
}



const u32 settingsButton::innerColor = RGBA8_MAXALPHA(244, 223, 71);
const u32 settingsButton::innerColorHovered = RGBA8_MAXALPHA(247, 231, 121);
const u32 settingsButton::borderColor = RGBA8_MAXALPHA(29, 30, 32);
const u32 settingsButton::size = 40;
const u32 settingItem::bottomMargin = 0;
const u32 settingItem::borderHeight = 3;


settingItem::settingItem(): scrObject() {
    canSelect = true;
    canHover = true;
    width = 100;
    height = 100;
    padding = 4;
    gbColor =  RGBA8_MAXALPHA(0, 0, 0);
    borderColor = RGBA8_MAXALPHA(255, 34, 12);
    smallFntColor = RGBA8_MAXALPHA(128, 128, 128);
    largeFntColor = RGBA8_MAXALPHA(0, 0, 0);
}
settingItem::settingItem(mPoint position): settingItem() { pos = position;}
settingItem::settingItem(u32 wid): settingItem() {
    width = wid;
    //height = hei;
}
void settingItem::draw(menu* mnu/*,font& bigFnt,font& smallFnt*/) {
    //the pos should be in the center of the item
    mPoint offset(pos.x - width / 2, pos.y - height / 2);
    //mPoint offset = pos;
    //draw main rect
    //    scr->drawCircle(mPoint(600,200),50,RGBA8_MAXALPHA(0,0,0));
    mnu->scr->drawRect(offset, mPoint(offset.x + width, offset.y + height), gbColor);
    //draw seperators # yes it re draws over the extra pixels i dont care
    mnu->scr->drawRect(offset, mPoint(offset.x + width, offset.y + borderHeight), borderColor);
    mnu->scr->drawRect(offset, mPoint(offset.x + width, offset.y + height - borderHeight), borderColor);

    //enum jsonTypes {J_NUMBER = 0, J_STRING, J_BOOL, J_ARRAY, J_OBJECT, J_NULL, J_UNKNOWN = -1};
    /*
        std::string smallText;
        switch(jsonObj.value[0]->type) {
            //I wish I know smart pointers
            case J_NUMBER:
                smallText = (dynamic_cast<jsonDataTypeNumber*>(jsonObj.value[0]))->getAsciiValue();
                break;
            case J_STRING:
                smallText = (dynamic_cast<jsonDataTypeString*>(jsonObj.value[0]))->getAsciiValue();
                break;
            case J_BOOL:
                smallText = (dynamic_cast<jsonDataTypeBool*>(jsonObj.value[0]))->getAsciiValue();
                break;
            case J_NULL:
                smallText = (dynamic_cast<jsonDataTypeNull*>(jsonObj.value[0]))->getAsciiValue();
                break;

            case J_ARRAY:
                break;
            case J_OBJECT:
                break;
            default:
                //cout << "memory Leak\n" ;
                //there is 100% a memory leak
                break;
        }
        //draw key texr
        mnu->scr->drawText(jsonObj.value[0]->getAsciiKey(),mPoint(offset.x+padding,offset.y+borderHeight+padding),bigFnt,false,largeFntColor);
        mnu->scr->drawText(smallText,mPoint(offset.x+padding+bigFnt.getTextLength(jsonObj.value[0]->getAsciiKey()),offset.y+borderHeight+padding),smallFnt,false,smallFntColor);
        //draw value text
    //    mnu->scr->drawText(,);
    */

}


settingsButton::settingsButton(): scrObject() {
    canSelect = true;
    canHover = true;

}
settingsButton::settingsButton(mPoint position): settingsButton() {
    pos = position;
}
void settingsButton::logic(menu* m_menu) {
    if((m_menu->mainController->kDown & KEY_A) /*|| (m_menu->mainController->kHeld & KEY_A)*/) {
        setMenu = new settingMenu(m_menu);
    }
}
void settingsButton::draw(menu* mnu) {
    if(this->isVisible) {
        mnu->scr->drawCircle(this->pos, this->size, this->borderColor);
        mnu->scr->drawCircle(this->pos, this->size - 3, (this->isHovered) ? innerColorHovered : innerColor);
    }
}

mainSettingsButton::mainSettingsButton(): settingsButton() {}
mainSettingsButton::mainSettingsButton(mPoint position): settingsButton(position) {}

void mainSettingsButton::logic(menu* m_menu) {

    if((m_menu->mainController->kDown & KEY_A) /*|| (m_menu->mainController->kHeld & KEY_A)*/) {
        //        setMenu = new mainSettingMenu(m_menu);
        setMenu  = new inputMenu(m_menu);
        std::vector<std::tuple<std::string, std::string, eventContainer*>> inputOptions{
            {"Edit RomFS Mods", "sans24", new menuEvent(m_menu, static_cast<void(menu::*)()>(&gameSelectMenu::centerGameCardsIfNeeded))},
            {"Edit ExeFs Mods", "sans24", new menuEvent(m_menu, &menu::dummy)},
            {"Toggle Moving", "sans24", new menuEvent(m_menu, &menu::dummy)},
            {"Hide Game", "sans24", new menuEvent(m_menu, &menu::dummy)},
            {"AnotherEvent", "sans24", new menuEvent(m_menu, &menu::dummy)}
        };
        //
        dynamic_cast<inputMenu*>(setMenu)->setOptions(inputOptions);
        //         inputMenu::addOptions(std::vector<std::tuple<std::string,std::string,u32 controlEvent>> &inputOptions)
    }


}
void modItem::draw(menu* mnu){
    mPoint offset(pos.x-(width/2),pos.y-(height/2));//sets offset to upper left corner of object

    if(this->isVisible) {
        //Todo Create info box as part of modItem draw, so I dont have to pass things to other things
        //This is how I should have done with the game info data
        //mnu->scr->drawRect(mPoint(0,0),mPoint(mnu->scr->width,mnu->scr->height),RGBA8(200,100,50,128));
        //Display icon
        if(iconImg->GetResult() == Jpeg::Decoder::OK && iconImg->IsColor()) {
            mnu->scr->drawRGB8(offset,iconImg->GetImage(), iconImg->GetWidth(), iconImg->GetHeight(),  width,height);
        }
        
    }
}
void modItem::logic(menu* mnu) {

    if(mnu->mainController->kDown & KEY_A){
        if(!isMoreOptionsButton){
            menu * setMenu  = new inputMenu(mnu);
            std::vector<std::tuple<std::string, std::string, eventContainer*>> inputOptions{
                {"Move Mod", "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::centerGameCardsIfNeeded))},
                {"Enable/select mod", "sans24", new menuEvent(mnu, &menu::dummy)},
                {"Delete Mod", "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::enableGameMoving))},
                {"ViewModFiles", "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::toggleHoveredGameCardScreen))},//deleteHoveredGameCard
                {"update mod", "sans24", new menuEvent(mnu, &menu::dummy)}
            };
            dynamic_cast<inputMenu*>(setMenu)->setOptions(inputOptions);
        }
        else{
            menu * setMenu  = new inputMenu(mnu);
            std::vector<std::tuple<std::string, std::string, eventContainer*>> inputOptions{
                {"Import Mod", "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::centerGameCardsIfNeeded))},
                {"Enable mods", "sans24", new menuEvent(mnu, &menu::dummy)},
                {"Delete All mods", "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::enableGameMoving))},
                {"Create LayeredMods", "sans24", new menuEvent(mnu, static_cast<void(menu::*)()>(&gameSelectMenu::toggleHoveredGameCardScreen))},//deleteHoveredGameCard
                {"Download Modpack", "sans24", new menuEvent(mnu, &menu::dummy)}
            };
            dynamic_cast<inputMenu*>(setMenu)->setOptions(inputOptions);
        }
        
    }

    //I can see some cool optimization going on here
    //todo
    //another option would be to move it left by a percentage of how much it needs to move. This would make it very fast  if 50% was used
    //issue may be infinityl small and alwasy me boving to get the last little part // not really an issue 
    
    
    //int leftLimit =  modItem::width/2 +modItem::gapBetweenGameCards;// + int(2*modItem::width/3 +  modItem::gapBetweenGameCards);
    //int rightLimit = mnu->wid - leftLimit;// int(mnu->scr->width - 2*modItem::width/3 - 2*modItem::gapBetweenGameCards);
//modItem
    //if(pos.x < leftLimit) {
        //another check to see if it is the first gamecard     gC.listPosId = gameCardCount
      //  int leftMost=pos.x
       // int rightMost=pos.x
       // for(int i=0;i<mnu->ScreenObjects.size();i++){
       //     if(mnu->ScreenObjects[i]->pos.x<leftMost){leftMost = mnu->ScreenObjects[i]->pos.x;}
        //    if(mnu->ScreenObjects[i]->pos.x>rightMost){rightMost = mnu->ScreenObjects[i]->pos.x;}
        //}
        
//        if(!leftMost==pos.x){//there is no other icons left of this hovered icon
            //dynamic_cast<modSelectMenu*>(mnu)->moveGCDS((leftLimit - pos.x) / (4));
 //       }
   //     else if(){
            
    //    }
//        else if
        
        
        
  //      if(this->listPosId == 0 && pos.x < int(modItem::width / 2) + int(modItem::width / 2 / 4)) {
  //          dynamic_cast<gameSelectMenu*>(mnu)->moveGCDS((int(modItem::width / 2) + int(modItem::width / 2 / 4) - pos.x) / 2);
  //      }

//        else if(//this != mnu->ScreenObjects[mnu->ScreenObjects.size() - dynamic_cast<gameSelectMenu*>(mnu)->gameCardCount]   ||
  //                  pos.x < int(modItem::width / 2) + int(modItem::width) + int(modItem::width / 2 / 4)) {
   //         dynamic_cast<modSelectMenu*>(mnu)->moveGCDS((leftLimit - pos.x) / (4));
    //    }
    //}
    //else if(pos.x > rightLimit) {
    //    dynamic_cast<modSelectMenu*>(mnu)->moveGCDS(-(pos.x - rightLimit) / (4));
    //}





}
modItem::modItem(): scrObject() {
   // gameTitleId = gameTitleID;
    //isMoreOptionsButton=false;
    canSelect = true;
    canHover = true;
    //isVisible = false;

//    gameTitleId=0;
   // settingsFileArrayPosId=0;

    std::string filePath = "romfs:/defaultModIcon.jpg";
    std::ifstream imgFile(filePath.c_str(), std::ios::binary | std::ios::ate);
    std::streamsize imgFileSize = imgFile.tellg();
    imgFile.seekg(0, std::ios::beg);

    char* imgFileBuff = new char [imgFileSize];
    imgFile.read(imgFileBuff, imgFileSize);
    imgFile.close();
    iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(imgFileBuff), imgFileSize);
    delete [] imgFileBuff;

    
}

int modItem::updateIcon(std::string filePath){
    if(filePath!= (settings::iconFolder + string("default"))){
        std::ifstream imgFile(filePath.c_str(), std::ios::binary | std::ios::ate);
        if(imgFile.is_open()){
            std::streamsize imgFileSize = imgFile.tellg();
            imgFile.seekg(0, std::ios::beg);

            char* imgFileBuff = new char [imgFileSize];
            imgFile.read(imgFileBuff, imgFileSize);
            imgFile.close();
            delete iconImg;
            iconImg = new Jpeg::Decoder(reinterpret_cast<unsigned char*>(imgFileBuff), imgFileSize);
            delete [] imgFileBuff;
        
        }
    }
    return 0;
}