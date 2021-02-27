#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if((argc!=3 || strcmp(argv[1],"-s")!=0)||(argc>1 && strcmp(argv[1],"-h")==0)||(argc>1 && strcmp(argv[1],"-help")==0)){
        printf("First paramether must be -s and than enter size limit in bytes\nExample: ./run.bin -s 25000");
        printf("Previous command will remove all files from folder that are smaller than 25000 bytes\n");
        return -1; 
    }
        
    char *ptr;
    int limit=strtol(argv[2],&ptr,10);

    DIR *dir=opendir(".");
    dirent *folder;
    while((folder=readdir(dir))!=NULL){
        FILE *f;
        char name[50];
        sprintf(name,"%s",folder->d_name);
        f=fopen(name,"r");

        fseek(f,SEEK_CUR,SEEK_SET);
        fseek(f,0,SEEK_END);
        int size=ftell(f);
        fseek(f,SEEK_CUR,SEEK_SET);

        bool rm=false;
        if(size!=-1)
            if(size<limit && strcmp(name,"run.bin")!=0){
                remove(name);
                rm=true;
            }
        
        char state[50];

        if(rm)
            strcpy(state,"removed");
        else
            strcpy(state,"");

        printf("%s\tsize: %d bytes, %s\n",name,size,state);

        fclose(f);
    }
        
    return 0;
}