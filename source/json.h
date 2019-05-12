#pragma once
#ifndef json_H
#define json_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>





enum jsonTypes {J_NUMBER = 0, J_STRING, J_BOOL, J_ARRAY, J_OBJECT, J_NULL, J_UNKNOWN = -1};










size_t findNextPosWithoutChars(std::string input, std::string bannedChars, size_t startPos, size_t endPos);
size_t findNextNonEscapedCharPos(std::string input, std::string searchChar, size_t startPos, size_t endPos);
size_t getBoundryEndPosFromBounderyChars(std::string input, std::string boundryStartChar, std::string boundryEndChar,
        size_t startPos, size_t endPos);
jsonTypes getJType(std::string input, size_t startPos);
size_t getJsonTypeEndPoint(std::string input, jsonTypes jType, size_t startPos, size_t endPos);




class jsonDataType {
  public:
    std::string key;
    //makes coding much easier, but may technically may be wasting space, but it might only be the space of a pointer as I think strings are on the heap not stack
    jsonTypes type;
    // I cant make this const for some reason
    jsonDataType(jsonTypes Type): type(Type) {
        key = "";
    };


    std::string getAsciiKey();

    virtual std::string getAsciiValue();
    virtual ~jsonDataType() {}
    //makes all children polymorphic dont delete

};

class jsonDataTypeString: public jsonDataType {
  public:
    std::string value;
    jsonDataTypeString();
    jsonDataTypeString(std::string val);
    std::string getAsciiValue();
    ~jsonDataTypeString()= default;
};

class jsonDataTypeNumber: public jsonDataType {
    // later I sould import my numbers lib that deals with exponents
  public:
    double value;

    jsonDataTypeNumber();
    jsonDataTypeNumber(double val);
    std::string getAsciiValue();
    ~jsonDataTypeNumber() = default;
};

class jsonDataTypeBool: public jsonDataType {
  public:
    bool value;
    jsonDataTypeBool();
    jsonDataTypeBool(bool val);
    ~jsonDataTypeBool() = default;
    std::string getAsciiValue();
};

class jsonDataTypeNull: public jsonDataType {
  public:
    std::string value;
    jsonDataTypeNull();
    std::string getAsciiValue();
    ~jsonDataTypeNull() = default;
};

class jsonDataTypeArray: public jsonDataType {
  public:

    std::vector<jsonDataType*> value;

    jsonDataTypeArray();
    jsonDataTypeArray(jsonTypes type);
    jsonDataTypeArray(jsonDataTypeArray* inJsonArray);
    

    void pushBack(jsonDataType* newElement);

    void pushBack(jsonDataTypeNumber jsonElement);
    void pushBack(jsonDataTypeString jsonElement);
    void pushBack(jsonDataTypeBool jsonElement);
    void pushBack(jsonDataTypeNull jsonElement);



    virtual ~jsonDataTypeArray();

    virtual std::string getAsciiValue();

};





class jsonDataTypeObject: public
    jsonDataTypeArray {
    //by making a json object just an array with keys it saves a lot of space and allows indexing
  public:

    jsonDataTypeObject();
    jsonDataTypeObject(jsonDataTypeObject* inJsonObject);

    bool containsKey(std::string checkKey);

    void pushBack(std::string inKey, jsonDataType* newElement);

    void pushBack(std::string inKey, jsonDataTypeNumber jsonElement);
    void pushBack(std::string inKey, jsonDataTypeString jsonElement);
    void pushBack(std::string inKey, jsonDataTypeBool jsonElement);
    void pushBack(std::string inKey, jsonDataTypeNull jsonElement);

    jsonDataType* key(std::string Key);
    std::string getAsciiValue();
    ~jsonDataTypeObject(){};
};


void importTextIntoJsonObj(std::string inputS, jsonDataTypeObject* jObj, size_t startPos, size_t endPos);
void importTextIntoJsonArr(std::string inputS, jsonDataTypeArray* jObj, size_t startPos, size_t endPos);


class jsonFileData {
  public:

    std::string file;
    std::string stringAtOpen;
    bool wasEverOpened;
    jsonDataTypeObject* data;
    int loadFile(std::string File);
    bool wasChangedSinceOpen();
    int saveFile();

    jsonFileData();
    ~jsonFileData();

};




#endif
