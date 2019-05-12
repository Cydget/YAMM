#include "json.h"





std::string jsonDataType::getAsciiKey() {
    return std::string("\"" + key + "\"");
}
std::string jsonDataType::getAsciiValue() {
    return std::string("Error! Function Not virtual for some reason!");

}

std::string jsonDataTypeString::getAsciiValue() {

    //std::string needsEscaping = "\'\"\?\\\a\b\f\n\r\t\v";
    std::string ret = value;

    for(size_t i = 0; i < ret.length(); ++i) { //hopefully ret.length() gets updated. I could see this being a bug on accident
        if(ret.at(i) == '\"') {
            ret.erase(i, 1);
            ret.insert(i, "\\\"");
        }
        //else if(ret.at(i) == '\'') {  ret.erase(i, 1); ret.insert(i, "\\\'");}
        else if(ret.at(i) == '{') {
            ret.erase(i, 1);
            ret.insert(i, "\\{");
        }
        else if(ret.at(i) == '}') {
            ret.erase(i, 1);
            ret.insert(i, "\\}");
        }
        else if(ret.at(i) == '[') {
            ret.erase(i, 1);
            ret.insert(i, "\\[");
        }
        else if(ret.at(i) == ']') {
            ret.erase(i, 1);
            ret.insert(i, "\\]");
        }
        else if(ret.at(i) == '\r') {
            ret.erase(i, 1);
            ret.insert(i, "\\r");
        }
        else if(ret.at(i) == '\f') {
            ret.erase(i, 1);
            ret.insert(i, "\\f");
        }
        else if(ret.at(i) == '\n') {
            ret.erase(i, 1);
            ret.insert(i, "\\n");
        }
        else if(ret.at(i) == '\t') {
            ret.erase(i, 1);
            ret.insert(i, "\\t");
        }
        else if(ret.at(i) == '\v') {
            ret.erase(i, 1);
            ret.insert(i, "\\v");
        }
    }

    return std::string("\"" + ret + "\"");


}
jsonDataTypeString::jsonDataTypeString(): jsonDataType(J_STRING) {}
jsonDataTypeString::jsonDataTypeString(std::string val): jsonDataTypeString() {
    //Here I need to remove all unescaped characters
    //then I have to convert all the escaped
    size_t count = 12;
    std::string needsRemoval[12] = {"\r", "\n", "\a", "\b", "\t", "\v", "\f",  "\"",  "{",  "}",  "[",  "]"};
    std::string replaceWith[12] = {"\\r", "\\n", "\\a", "\\b", "\\t", "\\v", "\\f", "\\\"", "\\{", "\\}", "\\[", "\\]"};

    for(size_t i = 0; i < count; ++i) {
        while(val.find(needsRemoval[i]) != std::string::npos) {
            val.erase(val.find(needsRemoval[i]), 1);
        }
    }

    for(size_t i = 0; i < count; ++i) {
        while(val.find(replaceWith[i]) != std::string::npos) {
            size_t pos = val.find(replaceWith[i]);
            val.erase(pos, replaceWith[i].length()); //always 2
            val.insert(pos, needsRemoval[i]);
        }
    }



    value = val;
}

jsonDataTypeNumber::jsonDataTypeNumber(): jsonDataType(J_NUMBER) {}
jsonDataTypeNumber::jsonDataTypeNumber(double val): jsonDataTypeNumber() {
    value = val;
}

std::string jsonDataTypeNumber::getAsciiValue() {
    std::stringstream retSS;
    retSS << std::setprecision(5) << value;
    return retSS.str();
}//this works for my purposes

jsonDataTypeBool::jsonDataTypeBool(): jsonDataType(J_BOOL) {}
jsonDataTypeBool::jsonDataTypeBool(bool val): jsonDataTypeBool() {
    value = val;
}


std::string jsonDataTypeBool::getAsciiValue() {
    return (value) ? "true" : "false";
}

jsonDataTypeNull::jsonDataTypeNull(): jsonDataType(J_NULL), value("null") {
}

std::string jsonDataTypeNull::getAsciiValue() {
    return "null";
}

jsonDataTypeArray::jsonDataTypeArray(): jsonDataType(J_ARRAY) {}


jsonDataTypeArray::jsonDataTypeArray(jsonTypes type): jsonDataType(type) {} //saves lots of space



std::string jsonDataTypeArray::getAsciiValue() {
    std::string ret = "[";

    for(size_t i = 0; i < this->value.size(); ++i) {
        if(i) {
            ret += ",";
        }

        ret += this->value[i]->getAsciiValue();
    }

    ret += "]";
    return ret;
}

jsonDataTypeObject::jsonDataTypeObject(): jsonDataTypeArray(J_OBJECT) {}

void jsonDataTypeObject::pushBack(std::string inKey, jsonDataType* newElement) {
    //this is eighter really efficient or a terrible way to do things
    if(!containsKey(inKey)) {
        jsonDataTypeArray::pushBack(newElement);
        this->value[this->value.size() - 1]->key = inKey;
    }

    //cout << inKey << endl;
}

void jsonDataTypeObject::pushBack(std::string inKey, jsonDataTypeNumber jsonElement) {
    pushBack(inKey, &jsonElement);
}
void jsonDataTypeObject::pushBack(std::string inKey, jsonDataTypeString jsonElement) {
    pushBack(inKey, &jsonElement);
}
void jsonDataTypeObject::pushBack(std::string inKey, jsonDataTypeBool jsonElement) {
    pushBack(inKey, &jsonElement);
}
void jsonDataTypeObject::pushBack(std::string inKey, jsonDataTypeNull jsonElement) {
    pushBack(inKey, &jsonElement);
}


jsonDataType* jsonDataTypeObject::key(std::string Key) {
    for(size_t i = 0; i < this->value.size(); ++i) {
        if(this->value[i]->key == Key) {
            return (this->value[i]);
        }
    }

    return this;
}

std::string jsonDataTypeObject::getAsciiValue() {
    std::string ret = "{";                    //json object format { key : value , ... }

    for(size_t i = 0; i < this->value.size(); ++i) {
        if(i) {
            ret += ",";
        }

        ret += this->value[i]->getAsciiKey();
        ret += ":";
        ret += this->value[i]->getAsciiValue();
    }

    ret += "}";
    return ret;
}






bool jsonDataTypeObject::containsKey(std::string checkKey) {
    for(size_t i = 0; i < this->value.size(); ++i) {
        if(this->value[i]->key == checkKey) {
            return true;
        }
    }

    return false;
}


jsonDataTypeArray::~jsonDataTypeArray() {
    for(size_t i = 0; i < value.size(); ++i) {
        //cout << dynamic_cast<jsonDataTypeString*>(this->value[i])->getAsciiValue() << endl;
        //cout << this->value[i]->type << endl;
        //this isnot actually needed and should be fixed to just be "delete this->value[i]"
        delete this->value[i];
        /*switch(this->value[i]->type) {
            case J_NUMBER:
                delete dynamic_cast<jsonDataTypeNumber*>(this->value[i]);
                break;

            case J_STRING:
                delete dynamic_cast<jsonDataTypeString*>(this->value[i]);
                break;

            case J_BOOL:
                delete dynamic_cast<jsonDataTypeBool*>(this->value[i]);
                break;

            case J_ARRAY:
                delete dynamic_cast<jsonDataTypeArray*>(this->value[i]);
                break;

            case J_OBJECT:
                delete dynamic_cast<jsonDataTypeObject*>(this->value[i]);
                break;

            case J_NULL:
                delete dynamic_cast<jsonDataTypeNull*>(this->value[i]);
                break;

            default:
                //there is 100% a memory leak
                break;
        }//end switch
        */
    }//end for loop
    value.clear();
}//end deconstruction


void jsonDataTypeArray::pushBack(jsonDataType* newElement) {
//this could be changed to see if the new element dynamically casts into the right pointer type
    switch(newElement->type) {
        //I wish I knew smart pointers
        case J_NUMBER:
            this->value.push_back(new jsonDataTypeNumber());
            *(dynamic_cast<jsonDataTypeNumber*>(this->value[this->value.size() - 1])) = *(dynamic_cast<jsonDataTypeNumber*>(newElement));
            break;

        case J_STRING:
            this->value.push_back(new jsonDataTypeString());
            *(dynamic_cast<jsonDataTypeString*>(this->value[this->value.size() - 1])) = *(dynamic_cast<jsonDataTypeString*>(newElement));
            break;

        case J_BOOL:
            this->value.push_back(new jsonDataTypeBool());
            *(dynamic_cast<jsonDataTypeBool*>(this->value[this->value.size() - 1])) = *(dynamic_cast<jsonDataTypeBool*>(newElement));
            break;

        case J_ARRAY://ON J_ARRAY AND J_OBJECT I NEED DEEP COPY, or maybe something weird
            //cout << "adding array" << endl;
            this->value.push_back(new jsonDataTypeArray(dynamic_cast<jsonDataTypeArray*>(newElement)));
            this->value[value.size() - 1]->key = newElement->key;
            break;

        case J_OBJECT:
            this->value.push_back(new jsonDataTypeObject(dynamic_cast<jsonDataTypeObject*>(newElement)));
            this->value[value.size() - 1]->key = newElement->key;
            //*(dynamic_cast<jsonDataTypeObject*>(this->value[this->value.size()-1])) = *(dynamic_cast<jsonDataTypeObject*>(newElement));
            break;

        case J_NULL:
            this->value.push_back(new jsonDataTypeNull());
            *(dynamic_cast<jsonDataTypeNull*>(this->value[this->value.size() - 1])) = *(dynamic_cast<jsonDataTypeNull*>(newElement));
            break;

        default:
            //cout << "memory Leak\n" ;
            //there is 100% a memory leak
            break;
    }

}


void jsonDataTypeArray::pushBack(jsonDataTypeNumber jsonElement) {
    jsonDataTypeArray::pushBack(&jsonElement);
}
void jsonDataTypeArray::pushBack(jsonDataTypeString jsonElement) {
    jsonDataTypeArray::pushBack(&jsonElement);
}
void jsonDataTypeArray::pushBack(jsonDataTypeBool jsonElement) {
    jsonDataTypeArray::pushBack(&jsonElement);
}
void jsonDataTypeArray::pushBack(jsonDataTypeNull jsonElement) {
    jsonDataTypeArray::pushBack(&jsonElement);
}










jsonDataTypeArray::jsonDataTypeArray(jsonDataTypeArray* inJsonArray): jsonDataTypeArray() {//deep copy
    for(size_t i = 0; i < inJsonArray->value.size() ; ++i) {
        pushBack(inJsonArray->value[i]);//gotta love recursion

    }//end for loop
}//end constructor

jsonDataTypeObject::jsonDataTypeObject(jsonDataTypeObject* inJsonObject): jsonDataTypeObject() {//deep copy
    for(size_t i = 0; i < inJsonObject->value.size() ; ++i) {
        jsonDataTypeArray::pushBack(inJsonObject->value[i]);//gotta love recursion
        //   std::cout << "push back key: " << inJsonObject->value[i]->key << std::endl;
    }//end for loop
}//end constructor








//start random functions







size_t findNextPosWithoutChars(std::string input, std::string bannedChars, size_t startPos = 0, size_t endPos = 0) {
    //forgive me father for I have sined. I dont check for unicode values
    if(endPos == 0) {
        endPos = input.length();
    }

    if(endPos != input.length()) {
        endPos++;
    }

    for(size_t i = startPos; i < endPos; ++i) {
        //std::cout << input.substr(i, 1);
        if(bannedChars.find(input.substr(i, 1)) == std::string::npos) {

            return i;
        }
    }

    return std::string::npos;
}


size_t findNextNonEscapedCharPos(std::string input, std::string searchChar, size_t startPos = 0, size_t endPos = 0) {
    if(endPos == 0) {
        endPos = input.length();
    }


    //std::cout << input;
    for(size_t i = startPos; i < endPos; ++i) {
        if(input.at(i) == searchChar.at(0) && (i == 0  ||  input.at(i - 1) != '\\')) {
            return i;
        }
    }

    return std::string::npos;
}

using namespace std;
size_t getBoundryEndPosFromBounderyChars(std::string input, std::string boundryStartChar, std::string boundryEndChar,
        size_t startPos, size_t endPos) {
    //input,startchar,endchar,endpos,retStartPos
    size_t retEndPos;
    unsigned int openBracketCount = 0;
    unsigned int closeBracketCount = 0;
    size_t tmpPos = startPos;
    bool shouldExit = false;

    do {//add something here
        //std::cout << "tmp pos:"  << tmpPos << std::endl;
        size_t lastFoundOpenPos  = findNextNonEscapedCharPos(input, boundryStartChar, tmpPos, endPos);
        size_t lastFoundClosePos = findNextNonEscapedCharPos(input, boundryEndChar, tmpPos, endPos);

        //cout << "openBracketCount: " << openBracketCount << endl;

        //cout << "closeBracketCount: " << closeBracketCount << endl;
        if(lastFoundOpenPos != std::string::npos && lastFoundOpenPos != std::string::npos) {
            if(lastFoundOpenPos < lastFoundClosePos) {
                openBracketCount++;
                tmpPos = lastFoundOpenPos + 1;
            }
            else {
                closeBracketCount++;
                tmpPos = lastFoundClosePos + 1;
            }
        }
        else if(lastFoundClosePos != std::string::npos) {
            closeBracketCount++;
            tmpPos = lastFoundClosePos + 1;
            //shouldExit = true;

        }
        else {
            tmpPos = std::string::npos;
            shouldExit = true;
            //will never find the end so it is a broken type catch all
        }
    }
    while(closeBracketCount  != openBracketCount   && !shouldExit);   //end while loop

    if(tmpPos != std::string::npos) {
        //std::cout << "case 1\n";
        retEndPos = tmpPos - 1;
        //cout << "ret end pos" << boundryStartChar<< retEndPos;

    }

    else {
        retEndPos = std::string::npos;
    }

    return retEndPos;


    //    cout << "the end pos of the obj is: " << retEndPos << endl;
    //    cout << "the start pos of the obj is: " << retStartPos << endl;

}



jsonTypes getJType(std::string input, size_t startPos) {

    jsonTypes ret = J_UNKNOWN;

    //std::cout << "get j type at" << input.substr(startPos) << std::endl;
    if(startPos != std::string::npos) {
        //retStartPos = startPos;
        if(input.at(startPos) == '{') {
            ret = J_OBJECT;
        }
        else if(input.at(startPos) == '\"') {
            ret = J_STRING;
        }
        else if(std::string("-0123456789").find(input.substr(startPos, 1)) != std::string::npos) {
            ret = J_NUMBER;
        }
        else if(input.substr(startPos, 4) == "true" ||
                input.substr(startPos, 5) == "false") { //spec says no caps so we dont need to check them
            ret = J_BOOL;
        }
        else if(input.at(startPos) == '[') {
            ret = J_ARRAY;
        }
        else if(input.substr(startPos, 4) == "null") {
            ret = J_NULL;
        }
    }

    return ret;
}

size_t getJsonTypeEndPoint(std::string input, jsonTypes jType, size_t startPos, size_t endPos = 0) {
    if(endPos == 0) {
        endPos = input.length();
    }

    size_t ret = std::string::npos;

    switch(jType) {
        case J_OBJECT:
            ret = getBoundryEndPosFromBounderyChars(input, "{", "}", startPos, endPos);
            break;

        case J_ARRAY:
            ret = getBoundryEndPosFromBounderyChars(input, "[", "]", startPos, endPos);
            break;

        case J_STRING:
            ret = findNextNonEscapedCharPos(input, "\"", startPos + 1, endPos) + 1;
            break;

        case J_NUMBER:

            ret = findNextPosWithoutChars(input, "+-0123456789Ee.", startPos, endPos);
            break;

        case J_BOOL:
            ret = startPos + 4;

            //I could use a ternary here but it would make it harder to read for example ret = (input.substr(startPos,5) == "false" )?startPos+4:startPos+3; //I could also set ret to start pos by default and then i wouldnt need the + i could do +=
            if(input.substr(startPos, 5) == "false") {
                ret += 1;
            }

            break;

        case J_NULL:
            ret = startPos + 4;//std::string("null").length();
            break;

        default:
            break;
    };


    return ret;
}






void importTextIntoJsonObj(std::string inputS, jsonDataTypeObject* jObj, size_t startPos = 0, size_t endPos = 0) {
    //startPos should be
    if(endPos == 0) {
        endPos = inputS.length();
    }

    std::string whiteSpace =    " " "\a" "\b" "\f" "\n" "\r" "\t" "\v";

    if(startPos == 0) {
        startPos = findNextPosWithoutChars(inputS, whiteSpace, startPos, endPos);
    }

    //std::cout << startPos <<std::endl;
    if(startPos != std::string::npos) {

        endPos = getJsonTypeEndPoint(inputS, J_OBJECT, startPos, endPos);
        //std::cout << "first ep " << endPos;

        if(endPos != std::string::npos) {


            bool continueLooking = true;

            while(continueLooking && startPos <= endPos) {

                startPos = findNextNonEscapedCharPos(inputS, "\"", startPos + 1, endPos) + 1;

                //std::cout << startPos <<std::endl;
                if(startPos != std::string::npos) {
                    size_t keyEndPos = findNextNonEscapedCharPos(inputS, "\"", startPos, endPos);

                    if(keyEndPos != std::string::npos) {
                        std::string key = inputS.substr(startPos, keyEndPos - startPos);
                        //std::cout << "key: " <<key <<std::endl;
                        //we know what a key is!! now to find out what type of data is associated with it
                        startPos = findNextPosWithoutChars(inputS, whiteSpace, keyEndPos + 1, endPos);

                        //	std::cout << "char after whitpos" <<inputS.at(startPos) << std::endl;
                        //startPos should now be a :
                        if(startPos != std::string::npos) {

                            startPos = findNextPosWithoutChars(inputS, whiteSpace, startPos + 1, endPos);
                            //   std::cout << "first pos of vale: " <<inputS.at(startPos) << std::endl;

                            if(startPos != std::string::npos) {

                                jsonTypes valueJType = getJType(inputS, startPos);
                                size_t valueEndPos = getJsonTypeEndPoint(inputS, valueJType, startPos, endPos);

                                //	cout << "End pos"<< valueEndPos;
                                //std::cout << "last pos of vale: " <<inputS.at(valueEndPos) << std::endl;
                                //std::cout << "string after value end pos " << inputS.substr(valueEndPos) << std::endl;
                                if(valueEndPos != std::string::npos) {
                                    //   cout << valueJType << endl;
                                    //   cout << "val end pos:" << valueEndPos << endl;
                                    std::string valueString = inputS.substr(startPos, valueEndPos - startPos);

                                    // std::cout << "value string:" << valueString << "\n";
                                    //   std::cout << valueJType << std::endl;

                                    switch(valueJType) {
                                        case J_NUMBER: {
                                                double numberValue = std::stod(valueString);
                                                jObj->pushBack(key, jsonDataTypeNumber(numberValue));
                                                //cout << inputS.at(startPos) <<endl;;
                                                //std::cout << "number: " << inputS.substr(valueEndPos) << std::endl;
                                                //startPos+=2000000;
                                            }
                                            break;

                                        case J_STRING: {
                                                valueString.erase(0, 1);
                                                valueString.erase(valueString.length(), 1);
                                                jObj->pushBack(key, jsonDataTypeString(valueString));
                                            }
                                            break;

                                        case J_BOOL: {
                                                // cout << "bool value st:" << valueString << endl;
                                                jObj->pushBack(key, jsonDataTypeBool(valueString == "true"));
                                            }
                                            break;

                                        case J_ARRAY: {
                                                //std::cout << "array Key:" << key << std::endl;
                                                jsonDataTypeArray* tmpJsonArray = new jsonDataTypeArray();
                                                //cout << inputS.substr(startPos,endPos) << endl;

                                                importTextIntoJsonArr(inputS, tmpJsonArray, startPos, endPos);
                                                //startPos++;
                                                jObj->pushBack(key, tmpJsonArray);
                                                //valueEndPos--;
                                                //std::cout << "endPos: " << valueEndPos;
                                                delete tmpJsonArray;
                                            }
                                            break;

                                        case J_OBJECT: {
                                                jsonDataTypeObject* tmpJsonObj = new jsonDataTypeObject();
                                                importTextIntoJsonObj(inputS, tmpJsonObj, startPos, endPos);
                                                jObj->pushBack(key, tmpJsonObj);
                                                delete tmpJsonObj;
                                            }
                                            break;

                                        case J_NULL:
                                            jObj->pushBack(key, jsonDataTypeNull());
                                            break;

                                        default:
                                            continueLooking = false;
                                            break;
                                    };

                                    //std::cout << key << std::endl;
                                    //std::cout  << key << "\n" ;
                                    startPos = findNextNonEscapedCharPos(inputS, ",", valueEndPos, endPos);

                                    // std::cout << inputS.substr(valueEndPos);
                                }
                                else {
                                    continueLooking = false;
                                }

                            }
                        }

                    }
                    else {
                        continueLooking = false;   //this needs a way to escape the loop if else
                    }
                }

            }//end loop
        }
    }


}


void importTextIntoJsonArr(std::string inputS, jsonDataTypeArray* jArr, size_t startPos = 0, size_t endPos = 0) {
    if(endPos == 0) {
        endPos = inputS.length();
    }

    std::string whiteSpace =    " " "\a" "\b" "\f" "\n" "\r" "\t" "\v";
    startPos = findNextPosWithoutChars(inputS, whiteSpace, startPos, endPos);

    if(startPos != std::string::npos) {
        endPos = getJsonTypeEndPoint(inputS, J_ARRAY, startPos, endPos);

        //cout << "array end ppos in array" << endPos;
        if(endPos != std::string::npos) {
            bool continueLooking = true;

            while(continueLooking && startPos < endPos) {
                startPos  = findNextPosWithoutChars(inputS, whiteSpace, startPos + 1, endPos);

                if(startPos != std::string::npos) {
                    //cout << "array value at first pos" << inputS.at(startPos);
                    jsonTypes valueJType = getJType(inputS, startPos);
                    size_t valueEndPos = getJsonTypeEndPoint(inputS, valueJType, startPos, endPos);

                    //			cout << valueEndPos;
                    if(valueEndPos != std::string::npos) {
                        std::string itemString = inputS.substr(startPos, valueEndPos - startPos);

                        switch(valueJType) {
                            case J_NUMBER: {
                                    double numberValue = std::stod(itemString);
                                    jArr->pushBack(jsonDataTypeNumber(numberValue));
                                }
                                break;

                            case J_STRING: {
                                    itemString.erase(0, 1);
                                    itemString.erase(itemString.length(), 1);
                                    jArr->pushBack(jsonDataTypeString(itemString));
                                }
                                break;

                            case J_BOOL: {
                                    jArr->pushBack(jsonDataTypeBool(itemString == "true"));
                                }
                                break;

                            case J_ARRAY: {
                                    jsonDataTypeArray* tmpJsonArray = new jsonDataTypeArray();
                                    //cout << inputS.substr(startPos-1,endPos) << endl;
                                    importTextIntoJsonArr(inputS, tmpJsonArray, startPos , endPos);
                                    jArr->pushBack(tmpJsonArray);
                                    delete tmpJsonArray;
                                }
                                break;

                            case J_OBJECT: {
                                    //cout << "got to object in array";
                                    jsonDataTypeObject* tmpJsonObj = new jsonDataTypeObject();
                                    importTextIntoJsonObj(inputS, tmpJsonObj, startPos, endPos);
                                    jArr->pushBack(tmpJsonObj);
                                    delete tmpJsonObj;
                                }
                                break;

                            case J_NULL:
                                jArr->pushBack(jsonDataTypeNull());
                                break;

                            default:
                                continueLooking = false;
                                break;
                        };

                        startPos = findNextNonEscapedCharPos(inputS, ",", valueEndPos, endPos);



                    }
                    else {
                        continueLooking = false;
                    }
                }
            }//end while
        }
    }


}








jsonFileData::jsonFileData(): wasEverOpened(false) {
    data = new jsonDataTypeObject();
}

int jsonFileData::loadFile(std::string File) {
    file = File;
    std::ifstream tmpFile;
    wasEverOpened = true;
    tmpFile.open(file.c_str());
    //for some reason it prefers c strings

    if(tmpFile.is_open()) {
        std::stringstream tmpFileBuffer;
        tmpFileBuffer << tmpFile.rdbuf();
        tmpFile.close();
        stringAtOpen = tmpFileBuffer.str();
        importTextIntoJsonObj(stringAtOpen, data, 0, 0);
    }
    else {
        return -1;
    }

    return 0;
}

bool jsonFileData::wasChangedSinceOpen() {
    return stringAtOpen != data->getAsciiValue();
}

int jsonFileData::saveFile() {
    //I could just use 1
    std::string currentAscii = this->data->getAsciiValue();

    if(!wasEverOpened || currentAscii != stringAtOpen) {
        ofstream fileOut;
        fileOut.open(file.c_str(), ios::out | ios::trunc);

        if(fileOut.is_open()) {
            fileOut << data->getAsciiValue();
        }
        else {
            fileOut.close();
            return -1;
        }
        fileOut.close();
        return 1;
    }

    return 0;
}


jsonFileData::~jsonFileData() {
    delete data;
}



















