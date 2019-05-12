  #pragma once
#ifndef iconUtils_H
#define iconUtils_H



#include <string.h>
#include <stdio.h>
#include <iostream>
#include <switch.h>
#include <iomanip>
#include <map>
#include <vector>
#include <cstring>
extern "C" {
#include <switch/services/fs.h>
#include <switch/services/lr.h>
#include <switch/services/ncm.h>
#include <switch/services/ns.h>
#include <switch/types.h>
    //#include <switch/services/set.h>
}

using namespace std;

enum titleType {
    NO_TITLE_FILTER,
    SYSTEM_TITLE,
    SYSTEM_DATA_ARCHIVE_TITLE,
    SYSTEM_UPDATE_TITLE,
    FIRM_PACK_A_TITLE,
    FIRM_PACK_B_TITLE,
    REG_APP_TITLE = 0x80,
    UPDATE_TITLE,
    ADDON_TITLE,
    DELTA_TITLE
};

const std::map<FsStorageId, std::string>FsStorageStrMap{
    {FsStorageId_None, "FsStorageId_None"},
    {FsStorageId_Host, "FsStorageId_Host"},
    {FsStorageId_GameCard, "FsStorageId_GameCard"},
    {FsStorageId_NandSystem, "FsStorageId_NandSystem"},
    {FsStorageId_NandUser, "FsStorageId_NandUser"},
    {FsStorageId_SdCard, "FsStorageId_SdCard"}
};
//https://switchbrew.org/index.php?title=Settings_services#Language
//Cant find a way to dynamically get all available language filenames from libnx only langCodes
const std::map<SetLanguage, std::string>SetLanguageIconMap{
    {SetLanguage_JA, "Japanese"},
    {SetLanguage_ENUS, "AmericanEnglish"},
    {SetLanguage_FR, "French"},
    {SetLanguage_DE, "German"},
    {SetLanguage_IT, "Italian"},
    {SetLanguage_ES, "Spanish"},
    {SetLanguage_ZHCN, "Chinese"},
    {SetLanguage_KO, "Korean"},
    {SetLanguage_NL, "Dutch"},
    {SetLanguage_PT, "Portuguese"},
    {SetLanguage_RU, "Russian"},
    {SetLanguage_ZHTW, "Taiwanese"},
    {SetLanguage_ENGB, "BritishEnglish"},
    {SetLanguage_FRCA, "CanadianFrench"},
    {SetLanguage_ES419, "LatinAmericanSpanish"}
};

Result getSysLang(s32& langOut);

Result setDarkMode();

Result getNcaPath(FsStorageId fsStoId, u64 titleID, std::string& pathOut);
Result getTitleData(u64 titleID, char* bufout, size_t bufSz, size_t& copiedSize, std::string& titleName) ;
Result importJpeg(FsStorageId fsStoId, u64 titleID, char* bufout, size_t& bufSz);
Result getTitleList(FsStorageId fsStorageType, std::vector<u64>& tidListOut, size_t& readSize);
std::string getTitleName(u64 titleID);

#endif


/*
  typedef enum
   {
       FsStorageId_None =       0,
       FsStorageId_Host =       1,
       FsStorageId_GameCard =   2,
       FsStorageId_NandSystem = 3,
       FsStorageId_NandUser   = 4,
       FsStorageId_SdCard     = 5,
   } FsStorageId

     for(unsigned int i=0;i<sizeof(NcmApplicationContentMetaKey);i++){
        printf("%02X ",reinterpret_cast<unsigned char*>(buf)[i]);
    }
*/

/* Notes [4.0.0+] The "normal" partition is now empty and a new partition "logo" was added.
 * The normal partition contains the .cnmt.nca and the game icondata nca. This is presumably for future compatibility so that if a future update changes the cryptographic protocol for the secure partition, Game icon data can still be shown in the home menu on old firmwares.


 *
 * The secure partition contains an identical copy of the .cnmt.nca and game icondata nca, as well as all other ncas required for the game.
 *
 *
 *
 *
 *
 * NCA Content FS
This page is for the FS which can be mounted for NCAs with OpenFileSystem*. Filesystem_services#OpenFileSystem|OpenFileSystem]] throws errors when trying to access a NCA-type with a FileSystemType not listed below.
 * NCA-type3
 *
*/

/*
//0x8000000000000041	ns	nsaccache:/	Home menu icondata/lru list for recently played games.
typedef enum
{
    FsFileSystemType_Logo = 2,
    FsFileSystemType_ContentControl = 3,
    FsFileSystemType_ContentManual = 4,
    FsFileSystemType_ContentMeta = 5,
    FsFileSystemType_ContentData = 6,
    FsFileSystemType_ApplicationPackage = 7
} FsFileSystemType;
*/
