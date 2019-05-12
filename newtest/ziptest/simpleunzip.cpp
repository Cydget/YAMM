#include <iostream>
#include <fstream>

#include <zzip/lib.h>

using namespace std;



int main(){

 ZZIP_DIR* dir = zzip_dir_open("test.zip",0);
 if (dir) {
   ZZIP_DIRENT dirent;
   if (zzip_dir_read(dir,&dirent) {
     /* show info for first file */ return 0;
     print("%s %i/%i", dirent.d_name, dirent.d_csize, dirent.st_size);}
   }
   zzip_dir_close(dir);
 }
 return 0;
}
