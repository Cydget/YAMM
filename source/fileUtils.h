#pragma once
#ifndef fileUtils_H
#define fileUtils_H

#include <iostream>
#include <switch.h>
#include <string>
#include <string.h>//needed for c strlen

#include <zzip/lib.h>
#include <fstream>

#include <filesystem>
namespace sfs = std::filesystem;

void extractZip(std::string zipLocation,std::string outputFolderLocation);
void myRecCopy(std::string oldFolder,std::string newFolder);//until fs::filesystem::copy compiles on switch
void createDirectories(std::string folderPath);
void copyFile(std::string src, std::string dest);


#endif
