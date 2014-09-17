#include <fstream>
#include "md5.h"
#include <vector>
#include <algorithm>
#include <dirent.h>

static std::vector<std::string> checksums;

std::string md5_file_checksum(std::string filename);
void do_ls(char *dirname);

int main(int argc, char** argv)
{
        do_ls(".");
        return(0);        
}



void do_ls(char *dirname)
{
    DIR *dir_ptr;
    dirent *direntp;
    if((dir_ptr=opendir(dirname))==NULL)
    {
        fprintf(stderr,"myls:cannot open %s\n",dirname);
    }else{
        while((direntp=readdir(dir_ptr))!=NULL){
            if(*(direntp->d_name) != '.' ){
                if(direntp->d_type == DT_DIR)
                {
                        /* TODO: Recoursive

                    char *newdir = new char[256];
                    memset(newdir, 0, sizeof(newdir));
                    strcpy(newdir, dirname);
                    if(newdir[strlen(dirname)-1] != '/') strcat(newdir, "/");
                    strcat(newdir, direntp->d_name);
                    do_ls(newdir);
                    delete[] newdir;
                    */
                }else
                if(direntp->d_type != DT_LNK)
                {
                        std::string md = md5_file_checksum(direntp->d_name);
                        //if(std::search(checksums.begin(), checksums.end(), md.begin(), md.end()) != checksums.end()) //don't work
                        if(std::find(checksums.begin(), checksums.end(), md) == checksums.end())
                        {
                                checksums.push_back(md);
                        }else{
                                std::cout<<"Duplicate: "<<direntp->d_name<<std::endl;
                                std::remove(direntp->d_name);
                        }
                }
            }
        }
    closedir(dir_ptr);
    }
}


std::string md5_file_checksum(std::string filename)
{
        std::ifstream file(filename.c_str(), std::ios_base::in);
        std::string data;
        while(!file.eof())
        {
                std::string str;
                file >> str;
                data += str;
        }
        file.close();

        return md5(data);
}