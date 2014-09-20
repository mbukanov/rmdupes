#include <fstream>
#include "md5.h"
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>


static std::vector<std::string> checksums;

std::string md5_file_checksum(std::string filename);
void do_ls(std::string dirname);

static bool Recoursive = false;
static bool bTemplate = false;
static bool bDirectory = false;
static std::string sDirectory;
static std::string sTemplate;

void showHelp()
{
    printf("%s\n", "Delete duplicates files");
    printf("%s\n", "rmdupes [KEY [arg]]");
    printf("%-10s - %s\n", "-r" , "Recoursive");
    printf("%-10s - %s\n", "-d [dir]", "Directory");
}

int main(int argc, char** argv)
{

    int rez = 0;
    while( (rez = getopt(argc, argv, "hrs:d:")) != -1 )
    {
        switch(rez)
        {
            case 'r': Recoursive = true; break;
            case 'h': showHelp(); exit(0); break;
            case 's': bTemplate = true; sTemplate = optarg; break;
            case 'd': bDirectory = true; sDirectory = optarg; break;
            case '?': printf("Error found !\n"); break;
        }
    }
    
    if(bDirectory)       do_ls((char*)sDirectory.c_str());
    else                 do_ls(".");

    return(0);        
}



void do_ls(std::string dirname)
{
    if(*(dirname.end()-1) != '/') dirname += "/";
    DIR *dir_ptr;
    dirent *direntp;
    if((dir_ptr=opendir(dirname.c_str()))==NULL)
    {
        fprintf(stderr,"myls:cannot open %s\n",dirname.c_str());
    }else{
        while((direntp=readdir(dir_ptr))!=NULL){
            if(*(direntp->d_name) != '.' ){
                if(direntp->d_type == DT_DIR && Recoursive)
                {
                    /* Recoursive */
                    std::string newdir = dirname + direntp->d_name;
                    do_ls(newdir);
                }else
                if(direntp->d_type != DT_LNK && direntp->d_type != DT_DIR)
                {
                        std::string md = md5_file_checksum(dirname+direntp->d_name);
                        if(std::find(checksums.begin(), checksums.end(), md) == checksums.end())
                        {
                                checksums.push_back(md);
                        }else{
                                std::cout<<"Duplicate: "<<dirname+direntp->d_name<<std::endl;
                                std::remove((dirname+direntp->d_name).c_str());
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