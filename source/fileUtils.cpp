#include "fileUtils.h"

void extractZip(std::string zipLocation,std::string outputFolderLocation){
    

    ZZIP_DIR* dir = zzip_dir_open(zipLocation.c_str(),0);
    if (dir) {
        ZZIP_DIRENT dirent;

        while (zzip_dir_read(dir,&dirent)) {
            if(dirent.d_name[strlen(dirent.d_name)-1]=='/' && dirent.st_size == 0){
                //this is a shit way of testing if its a directory but im going with it
                //looks like zzip_file_open only opens files as well, so it shouldnt be an issue. only problem is that an empty file ending with / will be treated as a folder, but im okay with that
                mkdir(std::string(outputFolderLocation+std::string(dirent.d_name)).c_str(), 0700);
            }
            else{//is file
                ZZIP_FILE* fp = zzip_file_open(dir,dirent.d_name,0);
                if(fp){
                    unsigned int buffSize = 200;//I should experiment to see what a good size should be
                    unsigned int totalRead = 0;
                    char * buf= new char[buffSize];
                    std::ofstream newFile;
                    newFile.open(std::string(outputFolderLocation+std::string(dirent.d_name)).c_str(),std::ios::out | std::ios::app | std::ios::binary);
                    if(newFile.is_open()){
                        zzip_ssize_t len = zzip_file_read(fp, buf, buffSize);
                        while(len){
                            totalRead+=len;
                            newFile.write(buf,len);
                            len = zzip_file_read(fp, buf, buffSize);
                        }
                    }
                    newFile.close();
                    delete[] buf;
                }//end if(fp)
                zzip_file_close(fp);
            }
        }
        zzip_dir_close(dir);
    }
    
}

void myRecCopy(std::string oldFolder,std::string newFolder){
    //UN-TESTED!!!
    //Important BOTH oldFolder and newFolder should end in /
    for(auto& p: sfs::recursive_directory_iterator(oldFolder)){
        //sfs::path relativePath = sfs::weakly_canonical(p.path()).lexically_relative(sfs::weakly_canonical(oldFolder)); //sfs::relative(p.path(),oldFolder);
        std::string relPath = p.path().string();
        if(relPath.find(oldFolder)){
            relPath.replace(relPath.find(oldFolder),oldFolder.length(),"");
        
            if(p.is_directory()){
                createDirectories(newFolder+relPath);
                //sfs::create_directories(sfs::path(newFolder+relPath) );
            }
            else if(p.is_regular_file()){
                copyFile(p.path().string(), newFolder+relPath);
                //sfs::copy_file(p.path(),sfs::path(newFolder+relPath));
            }
        }
        else{continue;}
//        std::cout << p.path() << '\n';
        //            sfs::copy_file(modJsonFilePath+iconName, settings::iconFolder+newIconName);
    }    
    
}

void createDirectories(std::string folderPath){
    std::string buildingPath="";
    std::string delim="/";
    long unsigned int delimPos = folderPath.find(delim);
    while(folderPath.size()){
        buildingPath+=folderPath.substr(0,delimPos+delim.size());
        //sfs::create_directory(buildingPath);
        mkdir(buildingPath.c_str(),0777);
        folderPath.erase(0,delimPos+delim.size());
        delimPos = folderPath.find(delim);
        if(delimPos==std::string::npos){delimPos=folderPath.size(); }
    }
}
void copyFile(std::string src, std::string dest){
    std::ifstream   inF( src.c_str(),std::ios::binary);    
    std::ofstream  outF(dest.c_str(),std::ios::binary);
    outF<<inF.rdbuf();
    

}

