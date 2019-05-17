#include "downloader.h"

double downloader::fileSize = 0;
double downloader::wroteCount = 0;

void downloader::setup(){
        
        curl_global_init(CURL_GLOBAL_DEFAULT);        
        curl_handle = curl_easy_init();
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        
        //debug stuff
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
        
        curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
        
}
int downloader::startDownload(){
            
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
size_t downloader::write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    //gets called multiple times i think
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    wroteCount+=written;
    //  std::cout << "wrote: "<<  written << std::endl;
    // std::cout << "filesize: " << fileSize << std::endl;
    // std::cout << "percent done: " << wroteCount/fileSize * 100 << "%" << std::endl ;
    //need to include a static pointer to the scr obj
    //last step is to create a function in scr that will show download progress 
    //it should take 2 args wroteCount and fileSize
    //it should call updateScreen after it is called
    
    
    return written;
}    
downloader::~downloader(){
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}
