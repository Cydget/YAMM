#include "fonts.h"




long unsigned int getCodePoint(std::string& input, size_t position, size_t& nextPosStart) {
    //this is a bit shitty, but it should get the job done (utf8 decoder)
    int byte = position;
    int byteCount = 0;
    long unsigned int val = 0;

    if(input.at(byte) > 128) {
        for(int i = 4; i > 0; --i) {
            if(input.at(byte) & (0x1 << (i + 3))) {
                byteCount++;
            }
            else {
                val =  input.at(byte) & ((((byteCount <= 1 ? 0x0C : 0) | (byteCount <= 2 ? 0x02 : 0) | (byteCount <= 3 ? 0x1 : 0)) << 3) | 0x07);
                i = 0;
            }
        }

        for(int i = 1; i < byteCount; i++) {
            val = (val << 6) | (input.at(byte + i) & 0x3F);
        }
    }

    if(byteCount) {
        byte += byteCount;
    }
    else {
        val = input.at(byte);
        byte++;
    }

    nextPosStart = byte;
    return val;
}
void removeNonAsciiChars(std::string& input) {
    for(size_t position = 0 ; position < input.length();) {
        size_t endPos = position;
        long unsigned int codePoint = getCodePoint(input, position, endPos);

        if(codePoint > 128) {
            input.erase(position, endPos - position);
            position++;
        }
        else {
            position = endPos;
        }
    }
}



font::font(std::string fontInfoFilePath) {
    fontInfo.loadFile(fontInfoFilePath);
    fontSize = dynamic_cast<jsonDataTypeNumber*>(
                           dynamic_cast<jsonDataTypeObject*>(
                                       fontInfo.data->key("config")
                           ) -> key("size")
               )->value;

    std::string textFile = dynamic_cast<jsonDataTypeString*>(
                                       dynamic_cast<jsonDataTypeObject*>(
                                               fontInfo.data->key("config")
                                       ) -> key("textureFile")
                           )->value;

    charHeight = dynamic_cast<jsonDataTypeNumber*>(
                             dynamic_cast<jsonDataTypeObject*>(
                                         fontInfo.data->key("config")
                             ) -> key("charHeight")
                 )->value;

    fontPNG = upng_new_from_file(("romfs:/fonts/" + textFile).c_str());

    if(fontPNG != NULL) {
        upng_decode(fontPNG);

        pngWidth = upng_get_width(this->fontPNG);
        pngHeight = upng_get_height(this->fontPNG);
        pngDepth = upng_get_bpp(this->fontPNG) / 8;

    }

}

font::~font() {
    upng_free(fontPNG);
}

//todo make this faster by makeing a function to getLetter width + xoffset instead of all the extra stuff
unsigned int font::getTextLength(std::string txt) {
    //returns the length of the max length line
    unsigned int tmpLen = 0;
    unsigned int largestLineLen = 0;


    for(size_t pos = 0 ; pos < txt.length();) {
        long unsigned int codePoint = getCodePoint(txt, pos, pos);
        int width = 0, height, pngXStart, pngYStart, xOffset = 0, yOffset, xAdvance = 0;

        if(codePoint == '\n') {
            if(tmpLen > largestLineLen) {
                largestLineLen = tmpLen;
            }

            tmpLen = 0;
        }
        else {
            getLetterInfo(codePoint, width, height, pngXStart, pngYStart, xOffset, yOffset, xAdvance);


            if(codePoint == ' ') {tmpLen += xAdvance / 2;}
            else {
                tmpLen += width + xOffset;
            }

            largestLineLen = (tmpLen > largestLineLen) ? tmpLen : largestLineLen;
        }
    }

    largestLineLen = (tmpLen > largestLineLen) ? tmpLen : largestLineLen;
    return largestLineLen;

}

/*
 *
 * get substring from start pos to eof or \n
 * check if whole string fits
 *   if not erase last char then try again
 * print text centered aka  -maxWid -(calcWid/2)
*/

std::string getFirstLine(std::string txt, size_t startPos) {
    std::string ret = txt.substr(startPos);

    for(size_t pos = startPos ; pos < txt.length();) {
        long unsigned int codePoint = getCodePoint(txt, pos, pos);

        if(codePoint == '\n') {
            ret = txt.substr(startPos, pos - startPos);
            pos = txt.length();
        }
    }

    return ret;
}





void font::getLetterInfo(long unsigned int codePoint, int& width, int& height, int& pngXStart, int& pngYStart, int& xOffset,
                         int& yOffset,
                         int& xAdvance) {
    jsonDataTypeArray* symbolsArray = dynamic_cast<jsonDataTypeArray*>(fontInfo.data->key("symbols")) ;

    for(unsigned int i = 0; i < symbolsArray->value.size(); i++) {
        unsigned int charId = dynamic_cast<jsonDataTypeNumber*>(
                                          dynamic_cast<jsonDataTypeObject*>(symbolsArray->value[i]) -> key("id")
                              )-> value;

        if(charId == codePoint) {
            jsonDataTypeObject* charObj = dynamic_cast<jsonDataTypeObject*>(symbolsArray->value[i]) ;
            width =       dynamic_cast<jsonDataTypeNumber*>(charObj -> key("width"))-> value;
            height =      dynamic_cast<jsonDataTypeNumber*>(charObj -> key("height"))-> value;
            pngXStart =   dynamic_cast<jsonDataTypeNumber*>(charObj -> key("x"))-> value;
            pngYStart =   dynamic_cast<jsonDataTypeNumber*>(charObj -> key("y"))-> value;
            xOffset =     dynamic_cast<jsonDataTypeNumber*>(charObj -> key("xoffset"))-> value;
            yOffset =     dynamic_cast<jsonDataTypeNumber*>(charObj -> key("yoffset"))-> value;
            xAdvance =     dynamic_cast<jsonDataTypeNumber*>(charObj -> key("yoffset"))-> value;
            i = symbolsArray->value.size();
        }

    }

}