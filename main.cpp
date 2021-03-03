#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>

struct condition{
    char type[3];
    long value;
    char *text;
};

condition *conn;
int c_count = 0;

void appendList(char *str);
void readSub(char *address);
void printHelp();
void printFileList();
void printError();
bool testFile(char *str);

bool _d = false;
bool _l = false;

int main(int argc, char *argv[]){

    if (argc == 1){
        printHelp();
        return -1;
    }

    int conditions = 0;
    for (int i = 1; i < argc; i++){

        if (strcmp("-h", argv[i]) == 0 || strcmp("-help", argv[i]) == 0){
            printHelp();
            return -1;
        }

        if (strcmp("-d", argv[i]) == 0){
            _d = true;
        }

        if (strcmp("-l", argv[i]) == 0){
            _l = true;
        }

        if (strcmp("-sb", argv[i]) == 0 || strcmp("-ss", argv[i]) == 0 
        || strcmp("-se", argv[i]) == 0 || strcmp("-ex", argv[i]) == 0
        || strcmp("-nc", argv[i]) == 0 || strcmp("-nr", argv[i]) == 0 
        || strcmp("-Cta", argv[i]) == 0 || strcmp("-Ctc", argv[i]) == 0
        || strcmp("-Ctr", argv[i]) == 0 || strcmp("-Ctb", argv[i]) == 0){
            conditions++;
        }
    }

    condition con[conditions];
    c_count = conditions;
    conn = con;

    int index = 0;
    for (int i = 1; i < argc; i++){

        if (strcmp("-sb", argv[i]) == 0 || strcmp("-ss", argv[i]) == 0 || 
        strcmp("-se", argv[i]) == 0){

            if (strcmp("-sb", argv[i]) == 0)
                strcpy(con[index].type, "-sb");
            else if (strcmp("-ss", argv[i]) == 0)
                strcpy(con[index].type, "-ss");
            else if (strcmp("-se", argv[i]) == 0)
                strcpy(con[index].type, "-se");

            if (argc <= i + 1){
                printError();
                return -1;
            }else{
                char *ptr;
                int value = strtol(argv[i + 1], &ptr, 10);
                con[index].value = value;
                index++;
            }
        }else if (strcmp("-ex", argv[i]) == 0 || strcmp("-nc", argv[i]) == 0
        || strcmp("-nr", argv[i]) == 0 || strcmp("-Cta", argv[i]) == 0 
        || strcmp("-Ctc", argv[i]) == 0 || strcmp("-Ctr", argv[i]) == 0
        || strcmp("-Ctb", argv[i]) == 0){

            if (strcmp("-ex", argv[i]) == 0)
                strcpy(con[index].type, "-ex");

            if (strcmp("-nc", argv[i]) == 0)
                strcpy(con[index].type, "-nc");

            if (strcmp("-nr", argv[i]) == 0)
                strcpy(con[index].type, "-nr");

            if (strcmp("-Cta", argv[i]) == 0)
                strcpy(con[index].type, "-Cta");

            if (strcmp("-Ctc", argv[i]) == 0)
                strcpy(con[index].type, "-Ctc");

            if (strcmp("-Ctr", argv[i]) == 0)
                strcpy(con[index].type, "-Ctr");

            if (strcmp("-Ctb", argv[i]) == 0)
                strcpy(con[index].type, "-Ctb");

            if (argc <= i + 1){
                printError();
                return -1;
            }else{
                con[index].text = argv[i + 1];
                index++;
            }
        }
    }

    DIR *dir = opendir(".");
    dirent *folder;
    while ((folder = readdir(dir)) != NULL){
        FILE *f;
        char name[strlen(folder->d_name)];
        memset(name, '\0', strlen(name));
        sprintf(name, "%s", folder->d_name);

        f = fopen(name, "r");

        fseek(f, SEEK_CUR, SEEK_SET);
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        fseek(f, SEEK_CUR, SEEK_SET);

        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0){
            if (size != -1)
                appendList(name);
            else{
                if (_d)
                    readSub((char *)name);
            }
        }

        fclose(f);
    }

    if (_l)
        printFileList();

    return 0;
}

void appendList(char *str){

    if (!testFile(str))
        return;

    if (strcmp(str, "temp_file_list.txt") == 0)
        return;

    FILE *f = fopen("temp_file_list.txt", "a");
    char newLine[1];
    sprintf(newLine, "%c", '\n');

    char temp[strlen(str)];
    sprintf(temp, "%s", str);

    fwrite(temp, sizeof(char), sizeof(temp), f);
    fwrite(newLine, sizeof(char), sizeof(newLine), f);
    fclose(f);
}

void readSub(char *address){

    DIR *dir = opendir(address);
    dirent *folder;
    while ((folder = readdir(dir)) != NULL){
        FILE *f;
        f = NULL;
        char name[strlen(address) + strlen(folder->d_name) + 1];
        memset(name, '\0', strlen(name));
        strcpy(name, address);
        strcat(name, "/");
        strcat(name, folder->d_name);

        if (strcmp(folder->d_name, ".") != 0 && strcmp(folder->d_name, "..") != 0)
            f = fopen(name, "r");

        if (f != NULL){
            fseek(f, SEEK_CUR, SEEK_SET);

            fseek(f, 0, SEEK_END);
            int size = ftell(f);
            fseek(f, SEEK_CUR, SEEK_SET);

            if (size != -1)
                appendList(name);
            else
                readSub((char *)name);

            fclose(f);
        }
    }
}

void printFileList(){
    FILE *f;
    f = fopen("temp_file_list.txt", "r");

    if (f == NULL){
        printf("There are no selected files.");
        return;
    }else{
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        fseek(f, 0, SEEK_SET);
        char buffer[size];
        fread(buffer, sizeof(char), size, f);
        fclose(f);
        printf("%s", buffer);
        remove("temp_file_list.txt");
    }
}

void printError(){
    printf("\nConditions not set properly. Type -h for help.\n");
}

void printHelp(){
    printf("\nFile Scalper is built for routine and fast tasks on large number of files.\n");
    printf("Per every execution it initializes two processes. First process finds all files based on given parameters.\n");
    printf("Second process executes command you have specified, on selected files.\n");

    printf("\n\t-h\tprints this help.\n");
    printf("\t-help\tsame as -t.\n\n");

    printf("Suport options:\n");
    printf("\t-d\tinclude subdirectories in search.\n");
    printf("\t-l\tlist all selected files.\n\n");

    printf("Search options:\n");
    printf("\t-sb\tfiles bigger than this value in size expressed in bytes.\n");
    printf("\t-ss\tfiles smaller than this value in size expressed in bytes.\n");
    printf("\t-se\tfiles with this exact size expressed in bytes.\n");
    printf("\t-ex\tfiles with extension expressed without dot.\n");
    printf("\t-nc\tfiles with text contained in filename.\n");
    printf("\t-nr\tfiles with names matched with regular expression.\n");
    printf("\t-Cta\t(text files only)files that contain any of provided texts.\n");
    printf("\t\tIf you want to search for multiple sentences, separate them with |||\n");
    printf("\t-Ctc\t(text files only)files that contain all of provided texts.\n");
    printf("\t\tIf you want to search for multiple sentences, separate them with |||\n");
    printf("\t-Ctr\t(text files only)files that contain at least one match by regular expression.\n");
    printf("\t-Ctb\t(binary files only).files that contain provided hexadecimal pattern.\n");

    printf("\n");
}

bool testFile(char *str){

    if(strcmp(str,"run.bin")==0)
        return false;

    FILE *f;
    f = fopen(str, "r");
    fseek(f, 0, SEEK_SET);
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fclose(f);

    bool result = true;
    for (int i = 0; i < c_count; i++){
        if (strcmp("-sb", conn[i].type) == 0)
            if (conn[i].value > size)
                result = false;

        if (strcmp("-ss", conn[i].type) == 0)
            if (result && conn[i].value < size)
                result = false;

        if (strcmp("-se", conn[i].type) == 0)
            if (result && conn[i].value != size)
                result = false;

        if (strcmp("-ex", conn[i].type) == 0)
            if (result && strlen(str) > 4){

                if (strstr(conn[i].text, " ")){

                    std::vector<std::string> vec;

                    char cc[strlen(conn[i].text)];
                    strcpy(cc, conn[i].text);
                    char *ptr = strtok(cc, " ");

                    while (ptr != NULL){
                        vec.push_back(ptr);
                        ptr = strtok(NULL, " ");
                    }

                    bool sub = false;
                    loop:for (int i1 = 0; i1 < vec.size(); i1++){
                        int l = vec[i1].length();
                        char c[l];
                        strcpy(c, vec[i1].c_str());
                        if (strcmp(str + strlen(str) - l, c) == 0){
                            sub = true;
                            break;
                        }
                    }

                    result = sub;
                }else{
                    if (strcmp(str + strlen(str) - 4, conn[i].text) != 0)
                        result = false;
                }
            }

        if (strcmp("-nc", conn[i].type) == 0)
            if (result && strlen(str) > 1){

                if (strstr(conn[i].text, " ")){

                    std::vector<std::string> vec;

                    char cc[strlen(conn[i].text)];
                    strcpy(cc, conn[i].text);
                    char *ptr = strtok(cc, " ");

                    while (ptr != NULL){
                        vec.push_back(ptr);
                        ptr = strtok(NULL, " ");
                    }

                    bool sub = false;
                    loop1:for (int i1 = 0; i1 < vec.size(); i1++){
                        int l = vec[i1].length();
                        char c[l];
                        strcpy(c, vec[i1].c_str());
                        if (strstr(str,c)){
                            sub = true;
                            break;
                        }
                    }

                    result = sub;
                }else{
                    if (!strstr(str,conn[i].text))
                        result = false;
                }
            }

        if (strcmp("-nr", conn[i].type) == 0){
            
            std::string value(conn[i].text);
            std::string search(str);
            std::regex regex(value);
            std::smatch sm;
            std::regex_search(search,sm,regex);
            if(sm.length()>0)
                result= true;
            else
                result= false;
            
        }

        if (strcmp("-Cta", conn[i].type) == 0 || strcmp("-Ctc", conn[i].type) == 0
        || strcmp("-Ctr", conn[i].type) == 0){

            FILE *f;
            f=fopen(str,"r");
            fseek(f,0,SEEK_END);
            long size=ftell(f);
            fseek(f,0,SEEK_SET);
            char buf[size];
            fread(buf,sizeof(char),size,f);
            fclose(f);

            if(strcmp("-Ctr", conn[i].type) == 0){

                std::string value(conn[i].text);
                std::string search(buf);
                std::regex regex(value);
                std::smatch sm;
                std::regex_search(search,sm,regex);
                if(sm.length()>0)
                    result= true;
                else
                    result= false;

            }else{

                if (strstr(conn[i].text, "|||")){

                    std::vector<std::string> vec;

                    char cc[strlen(conn[i].text)];
                    strcpy(cc, conn[i].text);
                    char *ptr = strtok(cc, "|||");

                    while (ptr != NULL){
                        vec.push_back(ptr);
                        ptr = strtok(NULL, "|||");
                    }

                    bool sub = false;
                    loop2:for (int i1 = 0; i1 < vec.size(); i1++){
                        int l = vec[i1].length();
                        char c[l];
                        strcpy(c, vec[i1].c_str());

                    if(strcmp("-Cta", conn[i].type) == 0){
                        if (strstr(buf,c)){
                            sub = true;
                            break;
                        } 
                    }else if(strcmp("-Ctc", conn[i].type) == 0){
                        if (strstr(buf,c))
                            sub = true;
                        else{
                            sub = false;
                            break;
                        }  
                    }
                }
                result = sub;

            }else{
                if (!strstr(buf,conn[i].text))
                    result = false;
            }
        }
    }
    
        if(strcmp("-Ctb", conn[i].type) == 0){

            result=false;
            FILE *f;
            f=fopen(str,"rb");
            fseek(f,0,SEEK_END);
            long size=ftell(f);
            fseek(f,0,SEEK_SET);
            unsigned char buf[size];
            fread(buf,sizeof(char),size,f);
            fclose(f);
            unsigned char val[sizeof conn[i].text / 2];
            char *pos=conn[i].text;
            unsigned char *pos1=buf;

            for(int i=0;i<sizeof conn[i].text / 2;i++){
                sscanf(pos,"%2hhx", &val[i]);
                pos+=2;
            }

            int counter=0;
            for(int i=0;i<size;i++){
                float f=(float)i/size;
                int p=f*100;
                printf("comparing file %s, %d of %d; %d%\n",str, i, size,p);
                if(val[counter]==buf[i]){
                    counter++;
                    for(int i1=i+1;i1<size;i1++){
                        if(val[counter]==buf[i1]){
                            counter++;
                            if(counter==sizeof val/sizeof(unsigned char)){
                                result=true;
                                goto ll;
                            }
                        }else{
                            counter=0;
                            goto l1;
                        }
                    }
                }
                l1:system("clear");
            }
            ll:;
        }
    }

    return result;
}