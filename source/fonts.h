#pragma once
#ifndef fonts_H
#define fonts_H

#include <iostream>
#include <string>
#include <switch.h>
#include <map>
#include "json.h"
#include "upng.h"

//Used google's nato fonts
//https://github.com/googlei18n/noto-fonts under SIL Open Font License (OFL) v1.1, http://scripts.sil.org/OFL
long unsigned int getCodePoint(std::string& input, size_t position, size_t& nextPosStart);
void removeNonAsciiChars(std::string& input);
std::string getFirstLine(std::string txt, size_t startPos);
class font {
  public:

    upng_t* fontPNG;
    unsigned int fontSize;
    unsigned int charHeight;

    unsigned int pngWidth, pngHeight, pngDepth;

    jsonFileData fontInfo;
    void getLetterInfo(long unsigned int codePoint, int& width, int& height, int& pngXStart, int& pngYStart, int& xOffset,
                       int& yOffset,
                       int& xAdvance);

    font(std::string fontInfoFilePath);
    ~font();
    unsigned int getTextLength(std::string txt);

};
//until I can figure it out, it will contain a font ptr and not a font
typedef std::map<std::string, font*> fontSetMap;



#endif
