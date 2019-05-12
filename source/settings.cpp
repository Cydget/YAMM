#include "settings.h"

//These need to be in this cpp file, because header files are evil and cause memory leaks
const std::string settings::nameOfApp =                  "YAMM";

const std::string settings::ExpectedVersionString =      "1.0.1";
const std::string settings::versionKey =                 "version";
const std::string settings::gamesKey =                   "games";

const std::string settings::gameIdKey =                  "Game";
const std::string settings::displayKey =                 "display";
const bool        settings::defaultDisplayValue =        true;
const std::string settings::gamePositionInOrderKey =     "List Position";
const std::string settings::gameNameKey =                "name";//name of the game is the game of the name's name game
const std::string settings::gameGBColorKey =             "bg-color";//there can be two colors used for customizing display
const std::string settings::gameAccentColorKey =         "accent-color";//second color
const std::string settings::gameFolderNameKey =          "modsFolder";//folder that stores mods
const std::string settings::gameSelectedLayerdFsModKey = "selectedLayeredFsModId";//selectedLayeredFsModId
const std::string settings::gameSelectedLayerdExeModKey = "selectedLayeredExeFsModId";//selectedLayeredFsModId
const std::string settings::gameLayerdFsEnableStatusKey = "Enable-LayeredFs";
const std::string settings::gameLayerdExeEnableStatusKey = "Enable-ExeFs";
const std::string settings::fsModsKey =                    "FS-mods";
const std::string settings::exeModsKey =                    "EXEFS-mods";

const std::string settings::updateUrlKey =                    "UpdateUrl";

const std::string settings::importModTypeKey =                    "ModType";



const std::string settings::modFolderPathKey =           "FolderName";
const std::string settings::iconPathKey =                "IconName";//change this to iconName
const std::string settings::modNameKey =                 "ModName";
const std::string settings::modDescriptionKey =          "Description";

const std::string settings::DEFAULTBGCOLOR =             "[0,0,0]";
const std::string settings::DEFAULTACCCOLOR =            "[3,2,1]";
const std::string settings::defaultRomFsModPath = "./"+nameOfApp+"/titles/";

const std::string settings::jsonFileNameForModPack = "mod_info.json";

const std::string settings::iconFolder =                "./"+nameOfApp+"/Icons/";

const std::string settings::fsMitmTitlePath =          "fsMitmTitlePath";
const std::string settings::defaultfsMitmTitlePath =          "./atmosphere/titles/";


//I will need to add two folder paths, 
//actually this would be more complicated then it needs to be
//changing it to only the prefix ex "/atmosphere/titles/" or "/sxos/titles" or "/reinx/titles/"
//full path will be /atmosphere/titles/titleID/EXEFS and /atmosphere/titles/titleID/EXEFS
//1 for where loaded romfs mods will be placed
//1 for where loaded exefs mods will be placed
std::string getRelativePath(std::string path, std::string basePath){
    std::string ret="";    
    
    return ret;
}
void createDirectories(std::string folderPath){
    std::string buildingPath="";
    std::string delim="/";
    int delimPos = folderPath.find(delim);
    while(folderPath.size()){
        buildingPath+=folderPath.substr(0,delimPos+delim.size());
        //sfs::create_directory(buildingPath);
        mkdir(buildingPath.c_str(),0777);
        folderPath.erase(0,delimPos+delim.size());
        delimPos = folderPath.find(delim);
        if(delimPos==std::string::npos){delimPos=folderPath.size(); }
    }
}
void copyFile(std::string src, std::string dest){
    std::ifstream   inF( src.c_str(),std::ios::binary);    
    std::ofstream  outF(dest.c_str(),std::ios::binary);
    outF<<inF.rdbuf();
    

}
void myRecCopy(std::string oldFolder,std::string newFolder){
    //UN-TESTED!!!
    //Important BOTH oldFolder and newFolder should end in /
    for(auto& p: sfs::recursive_directory_iterator(oldFolder)){
        //sfs::path relativePath = sfs::weakly_canonical(p.path()).lexically_relative(sfs::weakly_canonical(oldFolder)); //sfs::relative(p.path(),oldFolder);
        std::string relPath = p.path().string();
        if(relPath.find(oldFolder)){
            relPath.replace(relPath.find(oldFolder),oldFolder.length(),"");
        
            if(p.is_directory()){
                createDirectories(newFolder+relPath);
                //sfs::create_directories(sfs::path(newFolder+relPath) );
            }
            else if(p.is_regular_file()){
                copyFile(p.path().string(), newFolder+relPath);
                //sfs::copy_file(p.path(),sfs::path(newFolder+relPath));
            }
        }
        else{continue;}
//        std::cout << p.path() << '\n';
        //            sfs::copy_file(modJsonFilePath+iconName, settings::iconFolder+newIconName);
    }    
    
}


settings::settings(std::string file):filePath(file){
    settingsFile.loadFile(filePath);
    verifyData();
    
}

settings::~settings(){
    settingsFile.saveFile();//maybe I shouldnt put this in the deconstructor
}

int settings::verifyData(){
    //verifyFirstRun();
    verifySettingVersion(0);
    verifyMITMPath(0);
    verifyGames(0);
    return 0;
}

int settings::verifyMITMPath(int run = 0){
    if(settingsFile.data->containsKey(fsMitmTitlePath)){
        jsonDataTypeString* jsonMITMPATHStrPtr = dynamic_cast<jsonDataTypeString*>(settingsFile.data->key(fsMitmTitlePath));
        
        std::string versionString = jsonMITMPATHStrPtr->value;
        if(versionString.length()<1 || versionString.length()>255){
            jsonMITMPATHStrPtr->value = defaultfsMitmTitlePath;
            
            //Do other things that might be needed to be done to convert versions
            return verifyMITMPath(run+1);
        }
        return 0;
    }
    else if(!run){//create jsonStr and set its value
        settingsFile.data->pushBack(fsMitmTitlePath, jsonDataTypeString(defaultfsMitmTitlePath));
        //verify that it actually got fixed
        return verifyMITMPath(run+1);
    }
    return -1;
}

int settings::verifySettingVersion(int run){
    //std::cout << "does it contain: " << settingsFile.data->containsKey(versionKey) << std::endl;
    if(settingsFile.data->containsKey(versionKey)){
        jsonDataTypeString* jsonVersionStrPtr = dynamic_cast<jsonDataTypeString*>(settingsFile.data->key(versionKey));
        std::string versionString = jsonVersionStrPtr->value;
        //std::cout << versionString << std::endl;
        if(versionString!=ExpectedVersionString){
            jsonVersionStrPtr->value = ExpectedVersionString;
            //Do other things that might be needed to be done to convert versions
            return verifySettingVersion(run+1);
        }
        return 0;
    }
    else if(!run){//create jsonStr and set its value
        settingsFile.data->pushBack(versionKey, jsonDataTypeString(ExpectedVersionString));
        //verify that it actually got fixed
        return verifySettingVersion(run+1);
    }
    return -1;
}

/*
verifyhasKeyAndcorrectType(jsonDataType* jsonGamePtr,std::string theKey,jsonTypes jsonTYPE)
  if(settingsFile.data->containsKey(theKey)){
   * 
   * settingsFile.data->key(theKey)
    jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(theKey));
    if(jsonGameObjPtr==nullptr){//it exists but is wrong type
            delete settingsFile.data->key(theKey);
            jsonDataTypeObject tmp;
            settingsFile.data->pushBack(theKey, &tmp);
            jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(theKey));
    }
     
  }
  jsonTypes
enum jsonTypes {J_NUMBER = 0, J_STRING, J_BOOL, J_ARRAY, J_OBJECT, J_NULL, J_UNKNOWN = -1};


  
*/

int settings::getGameCardCount(){
    return dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey))->value.size();
}
//
bool settings::getModStatus(uint64_t gameId,std::string modTypeKey){
    if(containsGame(gameId)){
        return settings::getGameJsonBool(gameId, ((modTypeKey==fsModsKey)?settings::gameLayerdFsEnableStatusKey : settings::gameLayerdExeEnableStatusKey));
    }
    return false;
}
void settings::getModInfo(uint64_t gameId, std::string modTypeKey,int slotPosition,std::string &modName,std::string &folderName,std::string &iconPathName, std::string& description,std::string & versionString, std::string & updateUrl){
    if(containsGame(gameId)){
        jsonDataTypeArray* jsonModArrayPtr = 
            dynamic_cast<jsonDataTypeArray*>
           (dynamic_cast<jsonDataTypeObject*>
          (dynamic_cast<jsonDataTypeObject*>
          (dynamic_cast<jsonDataTypeObject*>
            (settingsFile.data->key(gamesKey))
            )->key(convertToHexString(gameId)))->key(modTypeKey));
            
        if(slotPosition<jsonModArrayPtr->value.size()){
            if(dynamic_cast<jsonDataTypeObject*>(jsonModArrayPtr->value[slotPosition])!=nullptr && dynamic_cast<jsonDataTypeObject*>(jsonModArrayPtr->value[slotPosition])->type == J_OBJECT ){
                jsonDataTypeObject* jsonModObjPtr = dynamic_cast<jsonDataTypeObject*>(jsonModArrayPtr->value[slotPosition]);
                modName       = settings::getModJsonString(jsonModObjPtr,modNameKey);//not sure if this will work as function was orriginaly made for import json objects
                folderName    = getModsFolderPath(gameId) +((modTypeKey==fsModsKey)?"romfs/":"exefs/") + settings::getModJsonString(jsonModObjPtr,modFolderPathKey);//should make the first part a function
                iconPathName  = settings::iconFolder + settings::getModJsonString(jsonModObjPtr,iconPathKey);//
                description   = settings::getModJsonString(jsonModObjPtr,modDescriptionKey);
                versionString = settings::getModJsonString(jsonModObjPtr,settings::versionKey);
                updateUrl     = settings::getModJsonString(jsonModObjPtr,settings::updateUrlKey);



//                &modName,std::string &folderName,std::string &iconPathName, std::string& description,std::string & versionString, std::string & updateUrl
                
            }
        }
        
     //modName,folderName, iconName, description
            //return jsonModArrayPtr->value.size();            
    }
}

int settings::getModCount(uint64_t gameId,std::string modTypeKey){
    if(containsGame(gameId)){
        jsonDataTypeArray* jsonModArrayPtr = 
        dynamic_cast<jsonDataTypeArray*>
       (dynamic_cast<jsonDataTypeObject*>
      (dynamic_cast<jsonDataTypeObject*>
      (dynamic_cast<jsonDataTypeObject*>
        (settingsFile.data->key(gamesKey))
        )->key(convertToHexString(gameId)))->key(modTypeKey));
        return jsonModArrayPtr->value.size();
    }
    return 0;//dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey))->value.size();
}
/*    if(modTypeArg==fsModsKey){modType="romfs";}
    else if(modTypeArg==settings::exeModsKey){modType="exefs";}*/

int settings::getGamesCount(){
    jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
    if(!jsonGameObjPtr->value.empty()){
        return jsonGameObjPtr->value.size();
    }
return 0;
}
int settings::getHiddenGamesCount(){
    return settings::getVectorOfHiddenGames().size();
}
int settings::getVisibleGamesCount(){
    return settings::getVectorOfVisibleGames().size();
}

std::vector<uint64_t> settings::getVectorOfVisibleGames(){
    jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
    std::vector<uint64_t>ret;
    if(!jsonGameObjPtr->value.empty()){
        for(int i= jsonGameObjPtr->value.size() -1 ;i>=0;i--){
                std::string str = dynamic_cast<jsonDataType*>(jsonGameObjPtr->value[i])->key;
                if(settings::getGameVisibility(stoull(str,0,16))){
                 ret.push_back(stoull(str,0,16));
                }
             
        }
    }
    return ret;
}
std::vector<uint64_t> settings::getVectorOfHiddenGames(){
    jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
    std::vector<uint64_t>ret;
    if(!jsonGameObjPtr->value.empty()){
        for(int i= jsonGameObjPtr->value.size() -1 ;i>=0;i--){
                            std::string str = dynamic_cast<jsonDataType*>(jsonGameObjPtr->value[i])->key;
                if(!settings::getGameVisibility(stoull(str,0,16))){
                 ret.push_back(stoull(str,0,16));
                }
             
        }
    }
    return ret;
}
std::vector<uint64_t> settings::getVectorOfAllGames(){
    jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
    std::vector<uint64_t>ret;
    if(!jsonGameObjPtr->value.empty()){
        for(int i= jsonGameObjPtr->value.size() -1 ;i>=0;i--){
             std::string str = dynamic_cast<jsonDataType*>(jsonGameObjPtr->value[i])->key;
             ret.push_back(stoull(str,0,16));
       }
        
    }
    return ret;
}

int settings::verifyGames(int run){
    
    if(settingsFile.data->containsKey(gamesKey)){//jsonGameObjPtr!=nullptr
        jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
        if(jsonGameObjPtr==nullptr){//it exists but is wrong type
            delete settingsFile.data->key(gamesKey);
            jsonDataTypeObject tmp;
            settingsFile.data->pushBack(gamesKey, &tmp);
            jsonDataTypeObject* jsonGameObjPtr = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
        }
        
        if(!jsonGameObjPtr->value.empty()){
            for(int i= jsonGameObjPtr->value.size() -1 ;i>=0;i--){
                if(verifyGame(jsonGameObjPtr->value[i])){
                    //std::cout << "need to delete\n";
                    //Not sure why there is 2 deletes
                    delete jsonGameObjPtr->value[i];
                    jsonGameObjPtr->value.erase(jsonGameObjPtr->value.begin()+i);
               //DELETE from list if it doesnt pass
                
                }

            }
        }
        return 0;
    }
    else if(run == 0){//create jsonObj for games
        //jsonDataTypeObject//                                                jsonDataTypeObject* tmpJsonObj = new jsonDataTypeObject();
        jsonDataTypeObject tmp;
        settingsFile.data->pushBack(gamesKey, &tmp);
        //verify that it actually got fixed
        return verifySettingVersion(run+1);
    }
    return -1;
}
int settings::verifyGame(jsonDataType* jsonGamePtr){
    int rc = 0; 
    if(jsonGamePtr->type != J_OBJECT){ return -1;}
    jsonDataTypeObject* gamePtr = dynamic_cast<jsonDataTypeObject*>(jsonGamePtr);
    if((rc = verifyGameId(gamePtr))){ return 1;}
    if((rc = verifyGameOrderPosition(gamePtr))){ return 2;}
    if((rc = verifyGameName(gamePtr))){ return 3;}
    if((rc = verifyColor(gamePtr,gameGBColorKey))){ return 4;}
    if((rc = verifyColor(gamePtr,gameAccentColorKey))){ return 5;}
    if((rc = verifyModsFolder(gamePtr))){ return 6;}
    
    if((rc = verifySelectedFSModID(gamePtr))){ return 7;}
    if((rc = verifyLayeredFSModsEnabledStatus(gamePtr))){ return 8;}
    if((rc = verifyLayeredFSMods(gamePtr))){ return 9;}
    
    if((rc = verifySelectedEXEModID(gamePtr))){ return 10;}
    if((rc = verifyLayeredEXEModsEnabledStatus(gamePtr))){ return 11;}
    if((rc = verifyEXEMods(gamePtr))){ return 12;}
    
    if((rc = verifyGameDisplayValue(gamePtr))){ return 13;}
    
    return rc;
}
int settings::verifyGameDisplayValue(jsonDataTypeObject* jsonGamePtr){
    if(jsonGamePtr->containsKey(displayKey) && ( (jsonGamePtr->key(displayKey))->type == J_BOOL ) ){
            return 0;
    }
    return -1;
}
int settings::verifyGameId(jsonDataTypeObject* jsonGamePtr){
    std::string gameId = dynamic_cast<jsonDataType*>(jsonGamePtr)->key;
   // if(jsonGamePtr->containsKey(gameNameKey) && ( (jsonGamePtr->key(gameNameKey))->type == J_STRING ) ){
        if(jsonGamePtr->type == J_OBJECT && isValidHexStr(gameId)){
                return 0;
        }
    
   // }
    return -1;
}
int settings::verifyGameOrderPosition(jsonDataTypeObject* jsonGamePtr){
    //idea make sure list position is not douplicated
    if(jsonGamePtr->containsKey(gamePositionInOrderKey) && ( (jsonGamePtr->key(gamePositionInOrderKey))->type == J_NUMBER ) ){
        int gamePos = dynamic_cast<jsonDataTypeNumber*>(jsonGamePtr->key(gamePositionInOrderKey))->value;
        if( gamePos >= 0 ){
            dynamic_cast<jsonDataTypeNumber*>(jsonGamePtr->key(gamePositionInOrderKey))->value = round(gamePos);
            return 0;
        }
    }
    return -1;
}
int settings::verifyGameName(jsonDataTypeObject* jsonGamePtr){
    if(jsonGamePtr->containsKey(gameNameKey) && ( (jsonGamePtr->key(gameNameKey))->type == J_STRING ) ){
        std::string gameName = dynamic_cast<jsonDataTypeString*>(jsonGamePtr->key(gameNameKey))->value;
        if( gameName.length() >= 0 ){
            return 0;
        }
        //I put this here, because currently if it tries to save a game name that doesnt have any unicode stuff it's length will probs be 0
        return 0;
    }
    return -1;
}
int settings::verifyColor(jsonDataTypeObject* jsonGamePtr,std::string colorKey){
    if(jsonGamePtr->containsKey(colorKey) && ( (jsonGamePtr->key(colorKey))->type == J_ARRAY ) ){
        jsonDataTypeArray* jsonarrayPtr = dynamic_cast<jsonDataTypeArray*>(jsonGamePtr->key(colorKey));
        if(jsonarrayPtr!= nullptr && jsonarrayPtr->value.size() == 3 ){
            for(size_t i=0;i<(jsonarrayPtr->value).size();i++){
                if(jsonarrayPtr->value[i]->type != J_NUMBER ){
                    return -1;
                }
                double colorPart = dynamic_cast<jsonDataTypeNumber*>(jsonarrayPtr->value[i])->value ;
                if(colorPart >= 0 && colorPart <= 0xff){
                    dynamic_cast<jsonDataTypeNumber*>(jsonarrayPtr->value[i])->value = round(colorPart);
                }
                else{
                    dynamic_cast<jsonDataTypeNumber*>(jsonarrayPtr->value[i])->value = 0x80; // I guess its halfway inbetween
                }
            }
            return 0;
        }
        
    }
    return -1;
}

int settings::verifyModsFolder(jsonDataTypeObject* jsonGamePtr){
    if(jsonGamePtr->containsKey(gameFolderNameKey) && ( (jsonGamePtr->key(gameFolderNameKey))->type == J_STRING ) ){
        std::string gameName = dynamic_cast<jsonDataTypeString*>(jsonGamePtr->key(gameFolderNameKey))->value;
        if( gameName.length() >= 0 && gameName.length() <=  0xff){//paths shouldnt be longer than 255 chars
            return 0;         //I could add other checks here maybe?
        }
    }
    return -1;
}
int settings::verifySelectedEXEModID(jsonDataTypeObject* jsonGamePtr){
    if(jsonGamePtr->containsKey(gameSelectedLayerdExeModKey) && ( (jsonGamePtr->key(gameSelectedLayerdExeModKey))->type == J_NUMBER ) ){
        double selectModPos = dynamic_cast<jsonDataTypeNumber*>(jsonGamePtr->key(gameSelectedLayerdExeModKey))->value;
        if( selectModPos >= -1 ){
            dynamic_cast<jsonDataTypeNumber*>(jsonGamePtr->key(gameSelectedLayerdExeModKey))->value = round(selectModPos);
            return 0;
        }
    }
    return -1;
}
int settings::verifySelectedFSModID(jsonDataTypeObject* jsonGamePtr){
    if(jsonGamePtr->containsKey(gameSelectedLayerdFsModKey) && ( (jsonGamePtr->key(gameSelectedLayerdFsModKey))->type == J_NUMBER ) ){
        double selectModPos = dynamic_cast<jsonDataTypeNumber*>(jsonGamePtr->key(gameSelectedLayerdFsModKey))->value;
        if( selectModPos >= -1 ){
            dynamic_cast<jsonDataTypeNumber*>(jsonGamePtr->key(gameSelectedLayerdFsModKey))->value = round(selectModPos);
            return 0;
        }
    }
    return -1;
}

int settings::verifyLayeredFSModsEnabledStatus(jsonDataTypeObject* jsonGamePtr){
    if(jsonGamePtr->containsKey(gameLayerdFsEnableStatusKey) && ( (jsonGamePtr->key(gameLayerdFsEnableStatusKey))->type == J_BOOL ) ){
        return 0;
    }
    return -1;
}
int settings::verifyLayeredEXEModsEnabledStatus(jsonDataTypeObject* jsonGamePtr){
    
    if(jsonGamePtr->containsKey(gameLayerdExeEnableStatusKey) && ( (jsonGamePtr->key(gameLayerdExeEnableStatusKey))->type == J_BOOL ) ){
        return 0;
    }
    return -1;
}
int settings::verifyLayeredFSMods(jsonDataTypeObject* jsonGamePtr){
    if(jsonGamePtr->containsKey(fsModsKey) && ( (jsonGamePtr->key(fsModsKey))->type == J_ARRAY ) ){
        
        jsonDataTypeArray* jsonarrayPtr = dynamic_cast<jsonDataTypeArray*>(jsonGamePtr->key(fsModsKey));
        
        if(jsonarrayPtr!= nullptr  ){
            if(!jsonarrayPtr->value.empty()){
                for(int i= (jsonarrayPtr->value.size()-1) ; i >=0  ;i--){
                   // cout << i << endl;
                //   cout << jsonarrayPtr->value[i]->type << endl;

                if(jsonarrayPtr->value[i]->type != J_OBJECT || verifyLayeredFsMod(dynamic_cast<jsonDataTypeObject*>(jsonarrayPtr->value[i]))){
                        delete jsonarrayPtr->value[i];
                        jsonarrayPtr->value.erase(jsonarrayPtr->value.begin()+i);
                        //return -1;
                    }
                    //fix this to delete invalid modss
                    
                }
            }
            return 0;
        }
        
    }
    return -1;
}
int settings::verifyEXEMods(jsonDataTypeObject* jsonGamePtr){
    return verifyLayeredFSMods( jsonGamePtr);//ExeFs should essentially be the exact same structure as romfs mods
}

int settings::verifyLayeredFsMod(jsonDataTypeObject* modInfoObjectjson){
    int rc = 0;//why would I do this like this? I have no clue, and I do not understand
    if((rc = verifyModStringFromKey(modInfoObjectjson,modFolderPathKey))){ return 10;}
    if((rc = verifyModStringFromKey(modInfoObjectjson,modNameKey))){return 11;}
    if((rc = verifyModStringFromKey(modInfoObjectjson,modDescriptionKey))){return 12;}
    
    if((rc = verifyModStringFromKey(modInfoObjectjson,versionKey))){return 13;}
    if((rc = verifyModStringFromKey(modInfoObjectjson,updateUrlKey))){return 14;}
    if((rc = verifyModStringFromKey(modInfoObjectjson,iconPathKey))){return 15;}
    
    return rc;
}
int settings::verifyModStringFromKey(jsonDataTypeObject* modObjPtr,std::string checkingKey){
    if(modObjPtr->containsKey(checkingKey) && modObjPtr->key(checkingKey)->type == J_STRING ){
        return 0;
    }
    return -1;
}

std::string convertToHexString(uint64_t value){//kinda ugly, so try to avoid using it if possible
    std::stringstream hexStream;
    hexStream << std::setfill('0') << std::setw(16) << std::uppercase<< std::hex << value;
    return hexStream.str();
}

bool settings::containsGame(uint64_t gameTitle){
    jsonDataTypeObject * gamesObjPointer = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
    return gamesObjPointer->containsKey(convertToHexString(gameTitle));
}

int settings::getLastVisibleGameListPosistion(){//There may be a bug here, not gonna test it tho
        jsonDataTypeObject * gamesObjPointer = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
        
        auto gameWithMaxListPos = std::max_element(gamesObjPointer->value.begin(), gamesObjPointer->value.end(),
                                            [](jsonDataType*& t1, jsonDataType*& t2)->bool{
                                                jsonDataTypeNumber* d1 = dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(t1)->key(gamePositionInOrderKey));
                                                jsonDataTypeNumber* d2 = dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(t2)->key(gamePositionInOrderKey));
                                                return (d1 == nullptr || d2== nullptr )?true: (d1->value < d2->value) && (dynamic_cast<jsonDataTypeBool*>(dynamic_cast<jsonDataTypeObject*>(t2)->key(settings::displayKey))->value) ;
                                             });
        return ((gamesObjPointer->value.end() != gameWithMaxListPos )?
                dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(*gameWithMaxListPos)->key(gamePositionInOrderKey))->value :
                0);
}

int settings::getLastGameListPosistion(){
        jsonDataTypeObject * gamesObjPointer = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
        
        auto gameWithMaxListPos = std::max_element(gamesObjPointer->value.begin(), gamesObjPointer->value.end(),
                                            [](jsonDataType*& t1, jsonDataType*& t2)->bool{
                                                jsonDataTypeNumber* d1 = dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(t1)->key(gamePositionInOrderKey));
                                                jsonDataTypeNumber* d2 = dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(t2)->key(gamePositionInOrderKey));
                                                return (d1 == nullptr || d2== nullptr )?true: d1->value < d2->value ;
                                             });
        return ((gamesObjPointer->value.end() != gameWithMaxListPos )?
                dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(*gameWithMaxListPos)->key(gamePositionInOrderKey))->value :
                0);
}
int settings::addGame(uint64_t gameTitle,std::string gameName){
    if(!containsGame(gameTitle)){//not necessary but why waste cycles
        jsonDataTypeObject gameObject;
        //
        
        std::string gameTitleIdHexString = convertToHexString(gameTitle);
        
        gameObject.pushBack(gamePositionInOrderKey,jsonDataTypeNumber(getLastGameListPosistion()+1 ));
        
        gameObject.pushBack(gameNameKey,jsonDataTypeString(gameName));
        gameObject.pushBack(displayKey,jsonDataTypeBool(defaultDisplayValue));
        
        //load default color values
        jsonDataTypeArray gbColorArray;//DEFAULTBCCOLOR
        importTextIntoJsonArr(DEFAULTBGCOLOR, &gbColorArray, 0, 0);
        jsonDataTypeArray accentColorArray;//DEFAULTBCCOLOR
        importTextIntoJsonArr(DEFAULTACCCOLOR, &accentColorArray, 0, 0);
        
        //add to obj
        gameObject.pushBack(gameGBColorKey,&gbColorArray);
        gameObject.pushBack(gameAccentColorKey,&accentColorArray);
        
        //defaultRomFsModPath will need to change to the current mods folder string
        std::string folderPathString =  defaultRomFsModPath +  gameTitleIdHexString + "/";
        
        gameObject.pushBack(gameFolderNameKey,jsonDataTypeString(folderPathString));
        
        gameObject.pushBack(gameSelectedLayerdFsModKey,jsonDataTypeNumber(-1));
        gameObject.pushBack(gameLayerdFsEnableStatusKey,jsonDataTypeBool(true));
        
        gameObject.pushBack(gameSelectedLayerdExeModKey,jsonDataTypeNumber(-1));
        gameObject.pushBack(gameLayerdExeEnableStatusKey,jsonDataTypeBool(true));
        
        jsonDataTypeArray fsModsArray;
        gameObject.pushBack(fsModsKey,&fsModsArray);
        
        jsonDataTypeArray exefsModsArray;
        gameObject.pushBack(exeModsKey,&exefsModsArray);
        
        //add to actual json memory
        //We dont have to worry about pushing the same game twice, because pushBack will keep original
        dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey))->pushBack(gameTitleIdHexString,&gameObject);
    
    }
    return 0;
}
int settings::importMod(std::string modJsonFilePath){
    //return 0;
    return settings::importMod(modJsonFilePath,0,"");
}
int settings::importMod(std::string modJsonFilePath,std::string modTypeArg){
    return settings::importMod(modJsonFilePath,0,modTypeArg);
}
int settings::importMod(std::string modJsonFilePath,uint64_t gameID){
    return settings::importMod(modJsonFilePath,gameID,"");
}
int settings::importMod(std::string modJsonFilePath,uint64_t gameID,std::string modTypeArg){//modpacks json file assumes modJsonFilePath ends in /
    jsonFileData modJsonFile;
    modJsonFile.loadFile(modJsonFilePath+settings::jsonFileNameForModPack);
    
    std::string gameIDstring = getModJsonString(modJsonFile.data,settings::gameIdKey);  
    //todo have a way to pass gameIDstring via argument
    
    //if(gameIDstring.substr(0,2)!="0x"){gameIDstring.insert(0,"0x");}
    if(gameID==0){//this way it can import with or without gameID being in the file 
        //eg. can import from main menu or in mod menu
        gameID = stoull(gameIDstring,0,16);
    }
    
    std::string modType = getModJsonString(modJsonFile.data,settings::importModTypeKey);
    if(modType=="romfs"){modTypeArg=fsModsKey;}
    else if(modType=="exefs"){modTypeArg=exeModsKey;}
    if(modTypeArg==fsModsKey){modType="romfs";}
    else if(modTypeArg==settings::exeModsKey){modType="exefs";}
    
    
    std::string modName = getModJsonString(modJsonFile.data,settings::modNameKey);
    std::string folderName = getModJsonString(modJsonFile.data,settings::modFolderPathKey);
    std::string iconName = getModJsonString(modJsonFile.data,settings::iconPathKey);
    std::string description = getModJsonString(modJsonFile.data,settings::modDescriptionKey);
    std::string versionString = getModJsonString(modJsonFile.data,settings::versionKey);
    std::string updateUrl = getModJsonString(modJsonFile.data,settings::updateUrlKey);
    
    
    
    //cout << endl << endl << modName << endl << folderName << endl << iconName << endl << description << endl << updateUrl<< endl <<modType <<gameID;
    if(
       (gameID != 0) &&
       ((modType=="exefs")||(modType=="romfs")) &&    //    (modType.c_str() % 'romfs'*'exefs')
       (sfs::exists(modJsonFilePath+folderName))
    ){
        
        if(!settings::containsGame(gameID)){
            settings::addGame(gameID,gameIDstring);
        }
        if(modName==""){modName="unknown mod name";}
        if(description==""){description="unknown description";}
        //if(updateUrl==""){}dont worry about this, because it should be up to the modpack creator to provide a valid url

        //std::string newfolderName = settings::addMod(gameID,((modType=="romfs")?settings::fsModsKey:settings::exeModsKey), modName,folderName, iconName, description);
        std::string storageModFolderPath = getModsFolderPath(gameID)
                        +((modType=="romfs")?"romfs/":"exefs/");
        std::string newIconName = "Icon_";
        int iconEnding = 0;
        if(!sfs::exists(modJsonFilePath+iconName)){
            iconName="default";
        }
//        return 0;
        //iconName ="default";
        if(iconName!="default"){
            bool contin=false;
            createDirectories(settings::iconFolder);
//            sfs::create_directories();
            while(sfs::exists(settings::iconFolder+newIconName+std::to_string(iconEnding)+".jpg")){
                iconEnding++;
            }

            //I had a bug where I forgot to add .string() after .filename(), and wrote it this way to avoid that, but now it should be fixed, but im leaving it this way anyways
            newIconName=newIconName+std::to_string(iconEnding)+".jpg";//should I only allow jpegs?
            
            //sfs::create_directories(settings::iconFolder);
            //sfs::create_directory("/switch/fuck/");
            //sfs::copy_file(modJsonFilePath+iconName, settings::iconFolder+newIconName);
            copyFile(modJsonFilePath+iconName, settings::iconFolder+newIconName);
            
            iconName= newIconName;
            //rename(modJsonFilePath+iconName,storageModFolderPath+iconName);
        }

        std::string newfolderName = addMod(gameID,modTypeArg, modName,folderName, iconName, description,versionString,updateUrl);
        
      //  cout << newfolderName;
        
        //sfs::create_directories(storageModFolderPath+newfolderName+"/");
        createDirectories(storageModFolderPath+newfolderName+"/");
        myRecCopy(modJsonFilePath+folderName+"/", storageModFolderPath+newfolderName+"/");
        
//        sfs::copy(modJsonFilePath+folderName, storageModFolderPath+newfolderName, sfs::copy_options::recursive);
        //Not sure why but copy seems to make it not compile
        //I want this to only be a temporary patch
        
        
        //sfs::rename(modJsonFilePath+folderName,storageModFolderPath+newfolderName);
        
    }

    return 0;
}
int settings::importUnknownMod(uint64_t gameID,std::string modTypeKey, std::string folderPath){
    if(sfs::exists(folderPath)) {       
        std::string newFolderName =  settings::addMod(gameID,modTypeKey,"Unknown_Name","Unknown_Mod_","default","Unknown_Description","Unknown_Version","No_UpdateUrl");
        //sfs::create_directories(getfsMitmTitlePath() +convertToHexString(gameID)+"/");
        std::string storedModFolderPath = getModsFolderPath(gameID) +((modTypeKey==fsModsKey)?"romfs/":"exefs/");
        createDirectories(storedModFolderPath);
        //sfs::create_directories(storedModFolderPath);
        if(sfs::exists(storedModFolderPath)){
            sfs::rename(folderPath,storedModFolderPath+newFolderName);
            return 0;
        }
    }
    
    return -1;
}

std::string convertStringToUint64_t(uint64_t value){
    std::stringstream hexStream;
    hexStream << std::hex << value;
    return hexStream.str();
}

/*        jsonDataTypeObject modObject;
        modObject.pushBack(modNameKey,jsonDataTypeString(modName));
        modObject.pushBack(modDescriptionKey,jsonDataTypeString(description));
        modObject.pushBack(modFolderPathKey,jsonDataTypeString(folderName+std::to_string(ending)));
        modObject.pushBack(iconPathKey,jsonDataTypeString(iconName));*/
bool settings::getGameJsonBool(uint64_t gameID, std::string keyString){
    bool ret = false;
    if(containsGame(gameID)){
        return dynamic_cast<jsonDataTypeBool*>
                (dynamic_cast<jsonDataTypeObject*>
                    (dynamic_cast<jsonDataTypeObject*>
                        (settingsFile.data->key(gamesKey))
                        ->key(convertToHexString(gameID)))
                        ->key(keyString))->value;
    }
    return ret;
}
double settings::getGameJsonNumb(uint64_t gameID, std::string keyString){
    double ret = 0;
    if(containsGame(gameID)){
        return dynamic_cast<jsonDataTypeNumber*>
                (dynamic_cast<jsonDataTypeObject*>
                    (dynamic_cast<jsonDataTypeObject*>
                        (settingsFile.data->key(gamesKey))
                        ->key(convertToHexString(gameID)))
                        ->key(keyString))->value;
    }
    return ret;
}
std::string settings::getGameJsonString(uint64_t gameID, std::string keyString){
    std::string ret="";
    if(containsGame(gameID)){
        return dynamic_cast<jsonDataTypeString*>
                (dynamic_cast<jsonDataTypeObject*>
                    (dynamic_cast<jsonDataTypeObject*>
                        (settingsFile.data->key(gamesKey))
                        ->key(convertToHexString(gameID)))
                        ->key(keyString))->value;
    }
    return ret;
}

/*bool settings::getGameVisibility(uint64_t gameID){
    if(containsGame(gameID)){
        return dynamic_cast<jsonDataTypeBool*>(dynamic_cast<jsonDataTypeObject*>
        (dynamic_cast<jsonDataTypeObject*>
        (settingsFile.data->key(gamesKey))->key(convertToHexString(gameID)))->key(displayKey));
        dispBoolPtr->value;
    }
    return false;
}*/
std::string settings::getModJsonString(jsonDataTypeObject* modpackJsonObj,std::string keyVal){
    //only use with mod's individual json files modpackJsonObj
    //Well future me is ignoring past me^
    std::string ret="";
    if(modpackJsonObj->containsKey(keyVal) && modpackJsonObj->key(keyVal)->type == J_STRING  ){
        ret = dynamic_cast<jsonDataTypeString*>(modpackJsonObj->key(keyVal))->value;
        return ret;
    }
    return ret;
}

std::string settings::addMod(uint64_t gameID,std::string modTypeKey, std::string modName,std::string folderName, std::string iconName,std::string description,std::string version, std::string updateUrl){
    //the string this function returns is the folder name that it should be copied to after adjustments
    //cout << gameID << endl;
    if(containsGame(gameID)){
        
        //verifying when opening the configfile should be enough to 
                jsonDataTypeArray* jsonModArrayPtr = 
                dynamic_cast<jsonDataTypeArray*>
               (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
                (settingsFile.data->key(gamesKey))
                )->key(convertToHexString(gameID)))->key(modTypeKey));
        std::ofstream outF("./thetext.json");
        outF << jsonModArrayPtr->getAsciiValue();
        outF.close();
        
        //if(jsonModArrayPtr==nullptr){createDirectories("./itwasanullptr");}
        //dynamic_cast<jsonDataTypeObject*>
        //This is for when people try to import a folder who's name already exists
        folderName=folderName + "_";//add a seperator for the number to look nicer
        int ending = 0;
        bool continWhile=false;
        do{
            continWhile=false;
            for(int i= jsonModArrayPtr->value.size() -1 ;i>=0;i--){
                //createDirectories("./info/hasIvalue"+std::to_string(i));
                if( dynamic_cast<jsonDataTypeString*>(dynamic_cast<jsonDataTypeObject*>(jsonModArrayPtr->value[i])->key(modFolderPathKey))->value 
                    == std::string(folderName + std::to_string(ending)) 
                ){
                    i = jsonModArrayPtr->value.size() -1;//goes back and rechecks with new value 
                    ending++;
                }
            }
            std::string storedModFolderPath = getModsFolderPath(gameID) +((modTypeKey==fsModsKey)?"romfs/":"exefs/")+folderName;
            while(sfs::exists(storedModFolderPath+std::to_string(ending))){
                //this is for when users fuck up the folder layout 
                //technically this isnt the only thing necessary, because the loaded mod folder might not temp exist
                //It also allows for it to be done with as little file io as possible
                ending++;
                continWhile=true;
            }
        }while(continWhile);
        
        jsonDataTypeObject modObject;
        modObject.pushBack(modNameKey,jsonDataTypeString(modName));
        modObject.pushBack(modDescriptionKey,jsonDataTypeString(description));
        modObject.pushBack(modFolderPathKey,jsonDataTypeString(folderName+std::to_string(ending)));
        modObject.pushBack(iconPathKey,jsonDataTypeString(iconName));
        modObject.pushBack(versionKey,jsonDataTypeString(version));
        modObject.pushBack(updateUrlKey,jsonDataTypeString(updateUrl));


        jsonModArrayPtr->pushBack(&modObject);
        
        //return gamesObjPointer->containsKey(convertToHexString(gameTitle));
        
        return folderName+std::to_string(ending);
    }
    return folderName;
}
    int settings::swapGameListPosition(uint64_t gameTitle1,uint64_t gameTitle2){
        if(containsGame(gameTitle1)&&containsGame(gameTitle2)){
            jsonDataTypeObject * gamesObjectPointer = dynamic_cast<jsonDataTypeObject*>(settingsFile.data->key(gamesKey));
            
            jsonDataTypeNumber* nptr1 = dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(gamesObjectPointer->key(convertToHexString(gameTitle1)))->key(gamePositionInOrderKey));
            jsonDataTypeNumber* nptr2 = dynamic_cast<jsonDataTypeNumber*>(dynamic_cast<jsonDataTypeObject*>(gamesObjectPointer->key(convertToHexString(gameTitle2)))->key(gamePositionInOrderKey));
            double tmpPos=nptr1->value;
            nptr1->value = nptr2->value;
            nptr2->value = tmpPos;
            return 0;
        }
        return -1;
    }
    
int settings::swapModPositions(uint64_t gameID,std::string modTypeKey,int position1,int position2){//position should be unsigned int
        if(containsGame(gameID)){
            jsonDataTypeArray* jsonModArrayPtr = 
                dynamic_cast<jsonDataTypeArray*>
               (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
                (settingsFile.data->key(gamesKey))
                )->key(convertToHexString(gameID)))->key(modTypeKey));
            double selectedModPos = getSelectedModPos(gameID,((modTypeKey==fsModsKey)?settings::gameSelectedLayerdFsModKey:settings::gameSelectedLayerdExeModKey));
                if(selectedModPos==position1){
                    setSelectedModPos(gameID,((modTypeKey==fsModsKey)?settings::gameSelectedLayerdFsModKey:settings::gameSelectedLayerdExeModKey),position2);
                }
                else if(selectedModPos==position2){setSelectedModPos(gameID,((modTypeKey==fsModsKey)?settings::gameSelectedLayerdFsModKey:settings::gameSelectedLayerdExeModKey),position1);}
            
                
            if(jsonModArrayPtr->value.size()>position1 && jsonModArrayPtr->value.size()>position2){
                std::swap(jsonModArrayPtr->value[position1],jsonModArrayPtr->value[position2]);
                return 0;
            }
        }
        return -1;
    }
bool settings::getGameVisibility(uint64_t gameID){
    if(containsGame(gameID)){
        return dynamic_cast<jsonDataTypeBool*>(dynamic_cast<jsonDataTypeObject*>
        (dynamic_cast<jsonDataTypeObject*>
        (settingsFile.data->key(gamesKey))->key(convertToHexString(gameID)))->key(displayKey))->value;
        //dispBoolPtr->value;
    }
    return false;
}
int settings::setGameVisibility(uint64_t gameID,bool val){
        if(containsGame(gameID)){
            jsonDataTypeBool * dispBoolPtr = 
            dynamic_cast<jsonDataTypeBool*>(dynamic_cast<jsonDataTypeObject*>
            (dynamic_cast<jsonDataTypeObject*>
            (settingsFile.data->key(gamesKey))->key(convertToHexString(gameID)))->key(displayKey));
            dispBoolPtr->value=val;
            return 0;}
        return -1;
    }

int settings::toggleGameVisibility(uint64_t gameID){
    return settings::setGameVisibility(gameID,!getGameVisibility(gameID));
}
int settings::removeMod(uint64_t gameID,std::string modTypeKey,int position){
        //uncomment the delete lines when I import this into main project 
        
        if(containsGame(gameID)){
                jsonDataTypeArray* jsonModArrayPtr = 
                dynamic_cast<jsonDataTypeArray*>
               (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
                (settingsFile.data->key(gamesKey))
                )->key(convertToHexString(gameID)))->key(modTypeKey));
                
                if(jsonModArrayPtr->value.size()>position){
                    //one more if for if it is currently selected
                    
                    
                    double selectedModPos = getSelectedModPos(gameID,((modTypeKey==fsModsKey)?settings::gameSelectedLayerdFsModKey:settings::gameSelectedLayerdExeModKey));
                    std::string fpath="./notARealLocation/";
                    if(selectedModPos==position){
                        setSelectedModPos(gameID,((modTypeKey==fsModsKey)?settings::gameSelectedLayerdFsModKey:settings::gameSelectedLayerdExeModKey),-1);
                        fpath = getfsMitmTitlePath() +convertToHexString(gameID)+"/"+((modTypeKey==fsModsKey)?"romfs/":"exefs/");
                    }else{
                        fpath = getModsFolderPath(gameID)
                                            +((modTypeKey==fsModsKey)?"romfs":"exefs")
                                            +"/"
                                            +dynamic_cast<jsonDataTypeString*>
                                            (dynamic_cast<jsonDataTypeObject*>
                                            (jsonModArrayPtr->value[position])
                                            ->key(modFolderPathKey))->value;

                    }
                      //cout << fpath << endl;
                    if(sfs::exists(fpath)){
                        sfs::remove_all(fpath);
                    }
//
                    //cppfs::remove_all(fpath);
                    delete jsonModArrayPtr->value[position];
                    jsonModArrayPtr->value.erase(jsonModArrayPtr->value.begin()+position);
                }
        }
        return -1;
    }
    
std::string settings::getModsFolderPath(uint64_t gameID){
    //assumes that the game exists
    std::string mFolderPath = 
            dynamic_cast<jsonDataTypeString*>
           (dynamic_cast<jsonDataTypeObject*>
          (dynamic_cast<jsonDataTypeObject*>
          (dynamic_cast<jsonDataTypeObject*>
            (settingsFile.data->key(gamesKey))
            )->key(convertToHexString(gameID)))->key(gameFolderNameKey))->value;
    return mFolderPath;
}
double settings::getSelectedModPos(uint64_t gameID,std::string selectedModTypePosKey){
    //assumes that the game exists
    double selectModPos = 
            dynamic_cast<jsonDataTypeNumber*>
           (dynamic_cast<jsonDataTypeObject*>
          (dynamic_cast<jsonDataTypeObject*>
          (dynamic_cast<jsonDataTypeObject*>
            (settingsFile.data->key(gamesKey))
            )->key(convertToHexString(gameID)))->key(selectedModTypePosKey))->value;
    return selectModPos;
}

int settings::setSelectedModPos(uint64_t gameID,std::string selectedModTypePosKey,int val){
    dynamic_cast<jsonDataTypeNumber*>
   (dynamic_cast<jsonDataTypeObject*>
  (dynamic_cast<jsonDataTypeObject*>
  (dynamic_cast<jsonDataTypeObject*>
    (settingsFile.data->key(gamesKey))
    )->key(convertToHexString(gameID)))->key(selectedModTypePosKey))->value = val;
    return 0;
}
std::string settings::getfsMitmTitlePath(){
        return dynamic_cast<jsonDataTypeString*>(settingsFile.data->key(fsMitmTitlePath))->value;
    }
bool isValidHexStr(std::string& inputKey){
    return (inputKey.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos);
}


int settings::loadMod(uint64_t gameID,std::string modTypeKey,int position){
    
    if(containsGame(gameID)){

        jsonDataTypeArray* jsonModArrayPtr = 
                dynamic_cast<jsonDataTypeArray*>
               (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
              (dynamic_cast<jsonDataTypeObject*>
                (settingsFile.data->key(gamesKey))
                )->key(convertToHexString(gameID)))->key(modTypeKey));

                if((jsonModArrayPtr->value.size()>position)||(position==-1)){
                    double currentSelectedModPos = getSelectedModPos(gameID,((modTypeKey==fsModsKey)?settings::gameSelectedLayerdFsModKey:settings::gameSelectedLayerdExeModKey));
                    std::string modFolderLoadFolder = getfsMitmTitlePath() +convertToHexString(gameID)+"/"+((modTypeKey==fsModsKey)?"romfs/":"exefs/");
                    //std::string fpath="/notARealLocation/";
                    if(currentSelectedModPos==position){return 0;}//already loaded?
                    else if(currentSelectedModPos == -1){
                    //we good to just copy folder in 
                    }
                    else if(jsonModArrayPtr->value.size()<=currentSelectedModPos){
                        //something is not good as we will not be able to copy back into a folder
//                        sfs::create_directories("sandbox/1/2/a");
                        createDirectories(getfsMitmTitlePath() +convertToHexString(gameID)+"/");
//                        sfs::create_directories(getfsMitmTitlePath() +convertToHexString(gameID)+"/");
                        if(sfs::exists(modFolderLoadFolder)){
                            settings::importUnknownMod(gameID,modTypeKey, modFolderLoadFolder);
                        }
//                      sfs::rename(modFolderLoadFolder,getfsMitmTitlePath() +convertToHexString(gameID)+"/OLDMODS/");                        
                        //I am actually considering just deleting the folders although that would result in bad public outlash
                    }
                    else{//std::filesystem::remove("from.txt");
                    //move existing loaded mod back
                        std::string selectedModFolderPath = getModsFolderPath(gameID)
                                                +((modTypeKey==fsModsKey)?"romfs":"exefs")
                                                +"/"
                                                +dynamic_cast<jsonDataTypeString*>
                                                (dynamic_cast<jsonDataTypeObject*>
                                                (jsonModArrayPtr->value[currentSelectedModPos])
                                                ->key(modFolderPathKey))->value;
                       // mv(src,dest)
                       createDirectories(getModsFolderPath(gameID)+((modTypeKey==fsModsKey)?"romfs/":"exefs/"));
                       // sfs::create_directories(getModsFolderPath(gameID)+((modTypeKey==fsModsKey)?"romfs/":"exefs/"));
                        if(sfs::exists(modFolderLoadFolder)){
                            if(sfs::exists(selectedModFolderPath)){
                               //there is a folder where there shouldnt be one, so we need to import a new unknown mod
                               settings::importUnknownMod(gameID,modTypeKey, modFolderLoadFolder);
                            }
                            else{
                                sfs::rename(modFolderLoadFolder,selectedModFolderPath);
                            }
                        }

                    }
                    if((position!=-1)){
                        std::string newSelectedModFolderPath = getModsFolderPath(gameID)
                                                    +((modTypeKey==fsModsKey)?"romfs":"exefs")
                                                    +"/"
                                                    +dynamic_cast<jsonDataTypeString*>
                                                    (dynamic_cast<jsonDataTypeObject*>
                                                    (jsonModArrayPtr->value[position])
                                                    ->key(modFolderPathKey))->value;
                        if(sfs::exists(modFolderLoadFolder)){
                            settings::importUnknownMod(gameID,modTypeKey, modFolderLoadFolder);
                        }
                        createDirectories(getfsMitmTitlePath() +convertToHexString(gameID)+"/");
                        //sfs::create_directories(getfsMitmTitlePath() +convertToHexString(gameID)+"/");
                        if(sfs::exists(newSelectedModFolderPath)){
                            sfs::rename(newSelectedModFolderPath,modFolderLoadFolder);
                        }
                        
                    }
                    setSelectedModPos(gameID,((modTypeKey==fsModsKey)?settings::gameSelectedLayerdFsModKey:settings::gameSelectedLayerdExeModKey),position);
                }
    }
    return -1;
}
