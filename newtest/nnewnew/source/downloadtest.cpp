/***************************************************************************
 * _ _ ____ _
 * Project ___| | | | _ \| |
 * / __| | | | |_) | |
 * | (__| |_| | _ <| |___
 * \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2018, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ /* <DESC>
 * Download a given URL into a local file named page.out.
 * </DESC>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <zzip/lib.h>
#include <switch.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

class downloader{//probably thread safe if only one instance of downloader exists at a time otherwise all things will go to shit // This is kinda like a global function, but i want to limit how much stuff i do in a shit way, but it is still 100% shit the way im doing this
    //it is possible to make this thread safe by creating a query of downloader requests and doing one after the last was finished, but the rest of my application is single threaded, and I dont think I will have users downloading more than 1 file at a time especially since this is a syncrounous download
    //returns 0 if good or 1 if bad
public:
    CURL *curl_handle;
    std::string filepath,url;
    static double fileSize;
    static double wroteCount;
    int res;
    downloader(std::string url,std::string filepath):url(url),filepath(filepath),res(0){//need to include a pointer to the active menu as well
        fileSize=0;
        wroteCount=0;
    }

    void setup(){
        curl_global_init(CURL_GLOBAL_DEFAULT);        
        curl_handle = curl_easy_init();
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        //debug stuff
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
        
        curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
        
       
        
    }
    int startDownload(){
            //We should check if we have access to the internet before starting download, but that should not be done inside this class
            FILE *outFile;
            outFile = fopen(filepath.c_str(), "wb");
            if(outFile) {
                curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, outFile);
                //getfile size
                res = curl_easy_perform(curl_handle);
                curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
                //start downlaod
                if(fileSize>0){//this could be a problem if the website doesnt give a file size
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &downloader::write_data);
                    res = curl_easy_perform(curl_handle);
                }
                else{
                    res= !res;
                }

               // std::cout << "filesize: " << fileSize << std::endl;

                fclose(outFile);
            }

        return res;
        
    }
    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
        //gets called multiple times i think
        size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
        wroteCount+=written;
        std::cout << "wrote: "<<  written << std::endl;
        std::cout << "filesize: " << fileSize << std::endl;
        std::cout << "percent done: " << wroteCount/fileSize * 100 << "%" << std::endl ;
        //need to include a static pointer to the scr obj
        //last step is to create a function in scr that will show download progress 
        //it should take 2 args wroteCount and fileSize
        //it should call updateScreen after it is called
        
        
        return written;
    }    
    ~downloader(){
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
    }
};
double downloader::fileSize = 0;
double downloader::wroteCount = 0;
class unzipper{
public:
    std::string filePath;
    unzipper(std::string filePath,std::string location){}
    
    
};
 
int main(int argc, char *argv[]) {


  
  consoleInit(NULL);

    //Move the cursor to row 16 and column 20 and then prints "Hello World!"
    //To move the cursor you have to print "\x1b[r;cH", where r and c are respectively
    //the row and column where you want your cursor to move
    //printf("\x1b[16;20HHello World!");
    //downloader d1("http://192.168.1.31:8080/newtestfile.txt","outputfile.html");
    //d1.setup();
    //d1.startDownload();
    std::string tmpfolder="./tmpFolder/";
    ZZIP_DIR* dir = zzip_dir_open("test4.zip",0);
    if (dir) {
    ZZIP_DIRENT dirent;
    printf("\x1b[0;0HHello World!");
    while (zzip_dir_read(dir,&dirent)) {
        //return 0;
       // printf("%s %i/%i", dirent.d_name, dirent.d_csize, dirent.st_size);}
        std::cout << "name: " << dirent.d_name << " size:  " << dirent.d_csize << " stsize: " << dirent.st_size <<  std::endl;
        if(dirent.d_name[strlen(dirent.d_name)-1]=='/' && dirent.st_size == 0){
            //this is a shit way of testing if its a directory but im going with it
            //looks like zzip_file_open only opens files as well, so it shouldnt be an issue. only problem is that an empty file ending with / will be treated as a folder, but im okay with that
            mkdir(std::string(tmpfolder+std::string(dirent.d_name)).c_str(), 0700);
        }
        else{//is file
            ZZIP_FILE* fp = zzip_file_open(dir,dirent.d_name,0);
            if(fp){
                unsigned int buffSize = 100;
                unsigned int totalRead = 0;
                if(buffSize > dirent.st_size){
                    buffSize = dirent.st_size;
                }
//                char buf[10];
                char * buf= new char[buffSize];
                std::ofstream newFile;
                newFile.open(std::string(tmpfolder+std::string(dirent.d_name)).c_str(),std::ios::out | std::ios::app | std::ios::binary);
                if(newFile.is_open()){
                    zzip_ssize_t len = zzip_file_read(fp, buf, buffSize);
                    while(len){
                        totalRead+=len;
                        newFile.write(buf,len);
                        
                        if((dirent.st_size-totalRead) < buffSize ){//if the amount left is smaller then the buffsize we change the size so we dont read too much 
                            buffSize = (dirent.st_size-totalRead) + 1;//just in case
                            delete[] buf;
                            buf = new char[buffSize];
                        }
                        
                        len = zzip_file_read(fp, buf, buffSize);
                    }
                }
                newFile.close();
                delete[] buf;
                    

            }
            zzip_file_close(fp);
            
            
        }
        
        
        
        
        
    }
    zzip_dir_close(dir);
    }
    
    
    std::cout << "fuck\n" ;
    
    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
  
  
  
  
  
  
  
}
