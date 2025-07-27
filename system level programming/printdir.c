#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
//function for handling input directory
void print_info(const char *path){
    //initialize var
    DIR *dir;
    struct dirent *ent;
    struct stat file_stat;
    char file_path[1024];
    int file_num=0;
    //avoid larger file might cause overflow
    unsigned int total_byte=0;
    errno=0;
    //open and print input dir
    dir=opendir(path);
    if(path[0]!='.'){
        printf("%s:\n",path );
    }
    //loop through each file in the input dir
    while((ent=readdir(dir))!=NULL){
        //ignore the "." files
        if(ent->d_name[0]!='.'){
            //construct the path
            strcpy(file_path,path);
            strcat(file_path,"/");
            strcat(file_path,ent->d_name);
            //acquire the info of the file
            if(lstat(file_path,&file_stat)==0){
                //print file infos
                printf("%ld\t%s\n", file_stat.st_size , ent->d_name );
                //ignore non-regular files
                if(S_ISREG(file_stat.st_mode)){
                    file_num+=1;
                    total_byte+=file_stat.st_size;
                }

            }else{
                perror("lstat");
                exit(EXIT_FAILURE);
            }

        }
        errno=0;
    }
    //error handling
    if(errno!=0){
        perror("readdir");
        exit(EXIT_FAILURE);
    }
    if(closedir(dir)!=0){
        perror("closedir");
    }
    printf("%d Files; %d Bytes\n", file_num,total_byte);
}

int main(int argc,char *argv[]){
    //if no input then use current directory
    if(argc==1){
        printf("%s:\n","current directory" );
        print_info(".");
    }else{
        for(int i=1;i<argc;i++){
            print_info(argv[i]);
        }
    }
    exit(EXIT_SUCCESS);
}