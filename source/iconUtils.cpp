
#include "iconUtils.h"

Result getSysLang(s32& langOut) {
    Result rc = 1;

    if((rc = setInitialize())) {
        setExit();
        return rc;
    }

    u64 langCode = 0;

    if((rc = setGetSystemLanguage(&langCode))) {
        setExit();
        return rc;
    }

    rc = setMakeLanguage(langCode, &langOut);
    setExit();
    return rc;
}

Result setDarkMode() {
    Result rc = 1;

    if(!(rc = setsysInitialize())) {
        rc = setsysSetColorSetId(ColorSetId_Dark);
    }

    setsysExit();
    return rc;
}


Result getNcaPath(FsStorageId fsStoId, u64 titleID, std::string& pathOut) {
    Result rc = 1;

    if((rc = lrInitialize())) {
        lrExit();
        return rc;
    }

    LrLocationResolver lrR;
    char pTh[FS_MAX_PATH] = {0};

    if((rc = lrOpenLocationResolver(fsStoId, &lrR))) {
        serviceClose(&lrR.s);
        lrExit();
        return rc;
    }

    if((rc = lrLrResolveApplicationControlPath(&lrR, titleID, pTh))) {
        serviceClose(&lrR.s);
        lrExit();
        return rc;
    }

    serviceClose(&lrR.s);
    pathOut = pTh;
    lrExit();
    return rc;
}

Result getTitleData(u64 titleID, char* bufout, size_t bufSz, size_t& copiedSize, std::string& titleName) {
    Result rc = nsInitialize();

    if(!rc) {
        NsApplicationControlData NsACTRDAT;
        size_t cpdSz;

        if(!(rc = nsGetApplicationControlData(1, titleID, &NsACTRDAT, sizeof(NsApplicationControlData), &cpdSz))) {
            size_t iconSz = cpdSz - sizeof(NsACTRDAT.nacp);

            if(cpdSz < sizeof(NsACTRDAT.nacp)) {
                rc = 1;
            }
            else if(bufSz >= iconSz) {
                memcpy(bufout, NsACTRDAT.icon, iconSz);
                copiedSize = iconSz;
                NacpLanguageEntry* langentry = NULL;

                //char name[0x201] = {0};
                if(!(rc = nacpGetLanguageEntry(&NsACTRDAT.nacp, &langentry)) && langentry != NULL) {
                    titleName = std::string(langentry->name);
                }

            }
        }

    }

    nsExit();
    return rc;
}


std::string getTitleName(u64 titleID) {
    Result rc = nsInitialize();
    std::string titleName="";
    if(!rc) {
        NsApplicationControlData NsACTRDAT;
        size_t cpdSz;

        if(!(rc = nsGetApplicationControlData(1, titleID, &NsACTRDAT, sizeof(NsApplicationControlData), &cpdSz))) {
          //  size_t iconSz = cpdSz - sizeof(NsACTRDAT.nacp);

            if(cpdSz < sizeof(NsACTRDAT.nacp)) {
                rc = 1;
            }
            //memcpy(bufout, jj.icon, iconSz);
            //copiedSize = iconSz;
            NacpLanguageEntry* langentry = NULL;

            //char name[0x201] = {0};
            if(!(rc = nacpGetLanguageEntry(&NsACTRDAT.nacp, &langentry)) && langentry != NULL) {
                titleName = std::string(langentry->name);
            }

        
        }

    }

    nsExit();
    return titleName;
}






Result importJpeg(FsStorageId fsStoId, u64 titleID, char* bufout, size_t& bufSz) {
    //unused, becuase I dont think it works when romfs is open before hand, or maybe it needs it to be open afterwards. I believe this would allow compatibility with old systems 1.0.0 before they used the new method.
    Result rc = 1;
    //return rc;
    //size_t bufSzBefore = bufSz ;
    bufSz = 0;

    if((rc = fsInitialize())) {
        return rc;
    }

    std::string contentPath = "";

    //set contentPath
    if((rc = getNcaPath(fsStoId, titleID, contentPath))) {
        fsExit();
        //return 0x444;
        return rc;
    }

    char cPathcharstart[FS_MAX_PATH] = {0};
    strcpy(cPathcharstart, contentPath.c_str());

    FsFileSystem fsFs;

    if((rc = fsOpenFileSystemWithId(&fsFs, titleID, FsFileSystemType_ContentControl, cPathcharstart/* contentPath.c_str()*/))) {
        fsFsClose(&fsFs);
        fsExit();
        //return 0x555;
        return rc;
    }

    FsDir fsFsDir;
    char rootPth[FS_MAX_PATH] = {0};
    strcpy(rootPth, (std::string("/")).c_str());

    if((rc = fsFsOpenDirectory(&fsFs, rootPth,/* int flags*/ FS_DIROPEN_FILE, &fsFsDir))) {
        fsDirClose(&fsFsDir);
        fsFsClose(&fsFs);
        fsExit();
        //return 0x666;
        return rc;
    }//0x2 is files 0x1 is dirs


    size_t dirEntryBufCount = 3;
    size_t totsEntries = 0;

    std::map<std::string, size_t> iconFiles;

    FsDirectoryEntry* dirEntrybuf = new FsDirectoryEntry[dirEntryBufCount];

    do {
        if((rc = fsDirRead(&fsFsDir, 0x1/*u64 inval*/, &totsEntries, dirEntryBufCount, dirEntrybuf))) {
            delete [] dirEntrybuf;
            fsDirClose(&fsFsDir);
            fsFsClose(&fsFs);
            fsExit();
            //return 0x777;
            return rc;
        }

        if(totsEntries) {
            for(size_t i = 0; i < totsEntries; ++i) {
                if(!strncmp(dirEntrybuf[i].name, "icon_", sizeof("icon_") - 1)) {
                    iconFiles.insert(std::pair<std::string, size_t>(dirEntrybuf[i].name, dirEntrybuf[i].fileSize));
                }

            }
        }

    }
    while(totsEntries);


    s32 lang = 0;

    if((rc = getSysLang(lang))) {
        delete [] dirEntrybuf;
        fsDirClose(&fsFsDir);
        fsFsClose(&fsFs);
        fsExit();
        return rc;
    }


    std::map<std::string, size_t>::iterator iconMapit;

    if(iconFiles.find("icon_" + SetLanguageIconMap.at((SetLanguage)lang) + ".dat") == iconFiles.end()) {
        if(iconFiles.size() >= 1) {
            iconMapit = iconFiles.begin();
        }
    }
    else {
        iconMapit = iconFiles.find("icon_" + SetLanguageIconMap.at((SetLanguage)lang) + ".dat");
    }


    if(iconMapit != iconFiles.end()) {

        char iconFPath[0x300/*FS_MAX_PATH*/] = {0};
        strncpy(iconFPath, (std::string("/" + iconMapit->first)).c_str(), (std::string("/" + iconMapit->first)).size());

        //char* imgBuf = new char[iconMapit->second];
        //size_t copiedSz = 0;

        FsFile jpggFile;
        rc = fsFsOpenFile(&fsFs, iconFPath, 0 | FS_OPEN_READ | FS_OPEN_WRITE, &jpggFile);
        //serviceClose(&jpggFile.s);
        bufSz = rc;
        /*
        if(false && !rc) {
             if(bufSzBefore >= iconMapit->second) {
                //This is a bit redundant to copy it into another buffer before sending it into the correct buffer
                //But, 0.1Mb of ram isnt too bad
                bufSz = 0x6969;

                if(!(rc = fsFileRead(&jpggFile, 0, imgBuf, iconMapit->second, &copiedSz))) {
                    if(copiedSz == iconMapit->second) {
                        memcpy(bufout, imgBuf, copiedSz);
                        bufSz = copiedSz; //serves two purposes, returns how much was read, or how much should be read if it wasnt big enough
                    }
                    else {
                        bufSz = 6970;
                        rc = 1;
                    }
                }
                else {
                    //bufSz = iconMapit->second+0xff;
                    rc = 0xffa1;
                }
            }
            else {
                bufSz = iconMapit->second;
                rc = 0xfff1;
            }
        }*/

        fsFileClose(&jpggFile);
        //delete [] imgBuf;
    }


    delete [] dirEntrybuf;
    fsDirClose(&fsFsDir);
    fsFsClose(&fsFs);

    fsExit();
    return rc;
}



Result getTitleList(FsStorageId fsStorageType, std::vector<u64>& tidListOut, size_t& readSize) {
//    readSize = 0;
    Result rc = 1;

    rc = ncmInitialize();

    NcmContentMetaDatabase testDb;
    //for less than 2.0.0 need to open with OpenIContentStorage before next line, maybe not?

    if(!(rc = ncmOpenContentMetaDatabase(fsStorageType, &testDb))) {

        unsigned int dynamicSize = 1;
        NcmApplicationContentMetaKey* buf = new NcmApplicationContentMetaKey[dynamicSize];

        u32 writtenEntriesCount = 0, totalEntriesCount = 0;

        do {
            rc = ncmContentMetaDatabaseListApplication(&testDb, static_cast<int>(NO_TITLE_FILTER), buf,
                    dynamicSize * sizeof(NcmApplicationContentMetaKey), &writtenEntriesCount, &totalEntriesCount);

            if(R_FAILED(rc)) {
                break;
            }
            else if(dynamicSize >= totalEntriesCount) {
                break;
            }
            else if(writtenEntriesCount < totalEntriesCount) {

                //create correct size buffer
                delete[] buf;
                dynamicSize = totalEntriesCount;
                buf = new NcmApplicationContentMetaKey[dynamicSize];
            }
        }
        while(writtenEntriesCount != totalEntriesCount);

        if(R_SUCCEEDED(rc)) {
            for(u32 i = 0; i < writtenEntriesCount; ++i) {
                tidListOut.push_back(buf[i].baseTitleId);
                readSize++;
            }
        }

        delete[] buf;
    }

    serviceClose(&testDb.s);
    ncmExit();
    return rc;
}
