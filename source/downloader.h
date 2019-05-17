#pragma once
#ifndef downloader_H
#define downloader_H

#include <iostream>
#include <switch.h>

//TODO Remove unneeded includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <curl/curl.h>
#include <string>



class downloader{//probably thread safe if only one instance of downloader exists at a time otherwise all things will go to shit // This is kinda like a global function, but i want to limit how much stuff i do in a shit way, but it is still 100% shit the way im doing this another way is with a mutexlock
    //it is possible to make this thread safe by creating a query of downloader requests and doing one after the last was finished, but the rest of my application is single threaded, and I dont think I will have users downloading more than 1 file at a time especially since this is a syncrounous download
    //returns 0 if good or 1 if bad
public:
    CURL *curl_handle;
    std::string url,filepath;
    static double fileSize;
    static double wroteCount;
    int res;
    downloader(std::string url,std::string filepath):url(url),filepath(filepath),res(0){//need to include a pointer to the active menu as well
        fileSize=0;
        wroteCount=0;
    }
    void setup();
    int startDownload();
    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
    ~downloader();
};



#endif
