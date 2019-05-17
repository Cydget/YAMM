//#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H
#include <sys/stat.h>
//used for file io stuff
#include <cstdint>
//#include <experimental/filesystem>
//namespace cppfs = std::filesystem;

#include <iostream>
#include <string>
#include "fileUtils.h"
#include <cmath>//for at least floor
#include <math.h>// for round()

//#include <time.h>
#include <string>
#include <sstream>
#include <iomanip>
using std::cout;
using std::endl;
#include <algorithm>
#include "json.h"
#include <filesystem>
namespace sfs = std::filesystem;
//#include <fstream>
//#include <cstdio>


/*
 * todo
 * add a bunch of fixer functions
 * for example what if someone changed the list position number and there were doups, or if they deleted a number
 * this can be mitigated via a function that gets next sequential position instead of assuming they are all there
 * */
//void myRecCopy(std::string oldFolder,std::string newFolder);//until fs::filesystem::copy compiles on switch
//void createDirectories(std::string folderPath);
//void copyFile(std::string src, std::string dest);

std::string convertToHexString(uint64_t value);
bool isValidHexStr(std::string& inputKey);


class settings{
public:
    std::string filePath;
    jsonFileData settingsFile;
    
    bool containsGame(uint64_t gameTitle);
    int getLastGameListPosistion();

    int verifyGameDisplayValue(jsonDataTypeObject* jsonGamePtr);
    std::vector<uint64_t> getVectorOfAllGames();
    std::vector<uint64_t> getVectorOfVisibleGames();
    std::vector<uint64_t> getVectorOfHiddenGames();
    bool getGameVisibility(uint64_t gameID);
    int toggleGameVisibility(uint64_t);
    
    int getLastVisibleGameListPosistion();
    int getGamesCount();
    int getHiddenGamesCount();
    int getVisibleGamesCount();
    
//    std::string 

    bool getGameJsonBool(uint64_t gameID, std::string keyString);
    bool getModStatus(uint64_t gameId,std::string modTypeKey);
    int getModCount(uint64_t gameId,std::string modTypeKey);

//    void getModInfo(uint64_t gameId, std::string modTypeKey);
    void getModInfo(uint64_t gameId, std::string modTypeKey,int slotPosition,std::string &modName,std::string &folderName,std::string &iconPathName, std::string& description,std::string & versionString, std::string & updateUrl);
    int addGame(uint64_t gameTitle,std::string gameName);
    int getGameCardCount();
    std::string addMod(uint64_t gameID,std::string modTypeKey, std::string modName,std::string folderName, std::string iconName,std::string description,std::string version, std::string updateUrl);
    
    /*Loading mods will be */
    int swapGameListPosition(uint64_t gameTitle1,uint64_t gameTitle2);
    int swapModPositions(uint64_t gameID,std::string modTypeKey,int position1,int position2);
    int setGameVisibility(uint64_t gameID,bool val);
    int loadMod(uint64_t gameID,std::string modTypeKey,int position);
    int removeMod(uint64_t gameID,std::string modTypeKey,int position);
    //jsonDataType
    int importUnknownMod(uint64_t gameID,std::string modTypeKey, std::string folderPath);
    std::string getModsFolderPath(uint64_t gameID);
    std::string getModJsonString(jsonDataTypeObject*,std::string);
std::string getGameJsonString(uint64_t gameID, std::string keyString);
    double getSelectedModPos(uint64_t gameID,std::string selectedModTypePosKey);
    int setSelectedModPos(uint64_t gameID,std::string selectedModTypePosKey,int val);
    std::string getfsMitmTitlePath();
    double getGameJsonNumb(uint64_t gameID, std::string keyString);
    int verifyData();
    int verifyFirstRun(){return 0;}
    int verifySettingVersion(int run);
    int verifyMITMPath(int run);
    int verifyGames(int run);
        int verifyGame(jsonDataType* jsonGamePtr);
            int verifyGameId(jsonDataTypeObject* jsonGamePtr);
            
                int verifyGameOrderPosition(jsonDataTypeObject* jsonGamePtr);
                int verifyGameName(jsonDataTypeObject* jsonGamePtr);
                int verifyColor(jsonDataTypeObject* jsonGamePtr,std::string colorKey);
                int verifyModsFolder(jsonDataTypeObject* jsonGamePtr);
                int verifySelectedFSModID(jsonDataTypeObject* jsonGamePtr);
                int verifySelectedEXEModID(jsonDataTypeObject* jsonGamePtr);
                int verifyLayeredFSModsEnabledStatus(jsonDataTypeObject* jsonGamePtr);
                int verifyLayeredEXEModsEnabledStatus(jsonDataTypeObject* jsonGamePtr);
                int verifyLayeredFSMods(jsonDataTypeObject* jsonGamePtr);
                int verifyEXEMods(jsonDataTypeObject* jsonGamePtr);
                
                int verifyLayeredFsMod(jsonDataTypeObject* modInfoObjectjson);
                    int verifyModStringFromKey(jsonDataTypeObject* modObjPtr,std::string checkingKey);
                    
                //int verifyExeFsMod(jsonDataTypeObject* modInfoObjectjson);
    
    //not exactly sure why default parameters werent working. I dont have time to fuck with them
    int importMod(std::string modJsonFilePath,std::string modTypeArg);
    int importMod(std::string modJsonFilePath,uint64_t gameID);
    int importMod(std::string modJsonFilePath);
    int importMod(std::string modJsonFilePath,uint64_t gameID,std::string modTypeArg);
    static const std::string iconPathKey;
    static const std::string nameOfApp;
    static const std::string ExpectedVersionString;
    static const std::string versionKey;
    static const std::string gamesKey;
    static const std::string displayKey;
    static const std::string gameIdKey;
    static const std::string gamePositionInOrderKey;
    static const std::string gameNameKey;
    static const std::string gameGBColorKey;
    static const std::string gameAccentColorKey;
    static const std::string gameFolderNameKey;
    static const std::string gameSelectedLayerdFsModKey;
    static const std::string gameSelectedLayerdExeModKey;
    static const std::string gameLayerdFsEnableStatusKey;
    static const std::string gameLayerdExeEnableStatusKey;
    static const bool        defaultDisplayValue;
    static const std::string fsModsKey;
    static const std::string exeModsKey;

    //
    static const std::string modFolderPathKey;
    static const std::string modNameKey;
    static const std::string modDescriptionKey;
    
    static const std::string defaultfsMitmTitlePath;
    static const std::string fsMitmTitlePath;
    static const std::string anotherTest;
    
    static const std::string DEFAULTBGCOLOR;
    static const std::string DEFAULTACCCOLOR;
    static const std::string defaultRomFsModPath;
    
    static const std::string jsonFileNameForModPack;
    static const std::string iconFolder;

    static const std::string updateUrlKey;
    static const std::string importModTypeKey;

    
    settings(std::string file);
    ~settings();

};
namespace ioHelper{
    
//    int createFolderPath(){
    
//    }
//    std::experimental::filesystem::

    //std::experimental::filesystem::rename(modFolderLoadFolder,getfsMitmTitlePath() +convertToHexString(gameID)+"/OLDMODS/");
}








class gSettings{//globalSettings
//singleton class https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
public:
    settings * main;
    //globalSettings
    static gSettings& getInstance(){
        static gSettings instance;
        return instance;
    }
    gSettings(gSettings const&) = delete;
    void operator=(gSettings const&) = delete;
    ~gSettings(){
        delete main;
        //cout << "deleted";
    }
    
private:
    gSettings(){
        main = new settings("./test3.txt");
        //cout << "created\n";
    }

//    globalSettings(globalSettings const&);
//    void operator=(globalSettings const&);
};








#endif // SETTINGS_H
