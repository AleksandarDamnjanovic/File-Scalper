#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include "print_support.h"
#include <sys/stat.h>

struct condition{
    char type[4];
    long value;
    char *text;
};

struct execute{
    char type[3];
    char *direction;
    char *text;
};

execute *exec;
int e_count=0;

condition *conn;
int c_count = 0;

void appendList(char *str);
void readSub(char *address);
void printHelp();
void printFileList();
void printError();
bool testFile(char *str);
void execution(char *address,char *selected);
void no_name_excution(char *address, char *action, char *add);
bool getFileNameContains(char *address, char *selected);
bool getFileNameContainsRegex(char *address, char *selected);
void moveCopy(char* address, char* direction, bool move);

bool _d = false;
bool _l = false;
bool _lr= false;
bool _D = false;
bool _r = false;
bool _Rr= false;

char *load;

int main(int argc, char *argv[]){

    if (argc == 1){
        printHelp();
        return -1;
    }

    int conditions = 0;
    int executions=0;
    for (int i = 1; i < argc; i++){

        if (strcmp("-h", argv[i]) == 0 || strcmp("-help", argv[i]) == 0){
            printHelp();
            return -1;
        }

        if (strcmp("-d", argv[i]) == 0)
            _d = true;

        if (strcmp("-rm", argv[i]) == 0)
            _r = true;

        if (strcmp("-l", argv[i]) == 0)
            _l = true;

        if (strcmp("-lr", argv[i]) == 0)
            _lr = true;

        if(strcmp("-rn", argv[i])==0)
            executions++;

        if(strcmp("-mv", argv[i])==0)
            executions++;

        if(strcmp("-cp", argv[i])==0)
            executions++;

        if(strcmp("-Rr", argv[i])==0){
            _Rr = true;
            executions++;
        }
            
        if(strcmp("-D", argv[i])==0){
            executions++;
            _D=true;
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

    execute ex[executions];
    e_count=executions;
    exec=ex;

    int index = 0;
    int exec_index=0;
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
        }else if(strcmp("-rn", argv[i]) == 0){

            /*Here are executions declared and execution type given*/
            if (strcmp("-rn", argv[i]) == 0)
                strcpy(ex[exec_index].type, "-rn");

            if (argc <= i + 1){
                printError();
                return -1;
            }else
                ex[exec_index].direction = argv[i + 1];
            
           /*Here are direction of executions*/

            if (argc <= i + 2){
                printError();
                return -1;
            }else{
                ex[exec_index].text = argv[i+2];
                exec_index++;
            }
                
        }else if(strcmp("-mv", argv[i]) == 0 || strcmp("-cp", argv[i]) == 0
        || strcmp("-D", argv[i]) == 0 || strcmp("-Rr", argv[i]) == 0){

            if (strcmp("-mv", argv[i]) == 0)
                strcpy(ex[exec_index].type, "-mv");

            if (strcmp("-cp", argv[i]) == 0)
                strcpy(ex[exec_index].type, "-cp");

            if (strcmp("-D", argv[i]) == 0)
                strcpy(ex[exec_index].type, "-D\0");

            if (strcmp("-Rr", argv[i]) == 0)
                strcpy(ex[exec_index].type, "-Rr");

            if (argc <= i + 1){
                printError();
                return -1;
            }else{
                ex[exec_index].direction = argv[i + 1];
                exec_index++;
            }
                
        }
    }

    DIR *dir=NULL;
    char *dd=NULL;
    if(_D){
        for(int i=0;i<e_count;i++)
            if(strcmp(ex[i].type,"-D\0")==0){
                dir= opendir(ex[i].direction);
                dd=ex[i].direction;
                goto here;
            }
    }else
        dir= opendir(".");

    here:

    if(dir==NULL){
        printf("Unexisting directory\n");
        return -1;
    }

    dirent *folder;
    while ((folder = readdir(dir)) != NULL){
        FILE *f;
        char name[strlen(folder->d_name)];
        memset(name, '\0', strlen(name));
        sprintf(name, "%s", folder->d_name);

        int si;
        if(dd==NULL)
            si=strlen(name);
        else
            si=strlen(name)+strlen(dd);

        const char *address;
        char addr[si];
        if(dd==NULL){
            f = fopen(name, "r");
            address=name;
            strcpy(addr,address);
        }else{
            std::string s=dd;
            std::string s1=name;
            s+=s1;
            address=s.c_str();
            strcpy(addr,address);
            f=fopen(address , "r");
        }

        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        fseek(f, SEEK_CUR, SEEK_SET);

        if (strcmp(addr, ".") != 0 && strcmp(addr, "..") != 0){
            if (size != -1)
                appendList((char*) addr);
            else{
                if (_d)
                    if(!strstr(addr,"/.") && !strstr(addr,"/.."))
                        readSub((char *)addr);
            }
        }

        fclose(f);
    }

    if(_lr){
        printf("%s",load);
        free(load);
    }

    if (_l)
        printFileList();

    return 0;
}

void appendList(char *str){

    if(!testFile(str))
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

bool getFileNameContains(char *address, char *selected){
    std::string add=address;
    std::string sel=selected;
    std::string s(add);

    std::smatch match;
    std::regex reg("[a-zA-Z0-9\\s\\_\\-\\+]+[\\.]?[a-zA-Z0-9]+$");

    std::sregex_iterator curent(s.begin(),s.end(),reg);
    std::sregex_iterator end;
    
    match=*curent;
    std::string fileName;
    fileName=match.str();

    char f_name[fileName.length()];
    strcpy(f_name,fileName.c_str());
    
    if(strstr(f_name,selected))
        return true;
    else
        return false;

}

bool getFileNameContainsRegex(char *address, char *selected){
    std::string add=address;
    std::string sel=selected;

    std::string folderName=std::regex_replace(add,std::regex("[a-zA-Z0-9\\s\\_\\-\\+]+[\\.]?[a-zA-Z0-9]+$"),"");
    std::string fileName=std::regex_replace(add,std::regex(folderName),"");

    std::regex regex(selected);
    std::smatch sm;
    std::regex_search(add,sm,regex);

    if(sm.length()>0)
        return true;
    else
        return false;

}

void moveCopy(char* address, char* direction, bool move){
    FILE *f;
    f=fopen(address,"rb");

    fseek(f,0,SEEK_END);
    int size= ftell(f);
    fseek(f,0,SEEK_SET);
    char buf[size];
    fread(buf,sizeof(char),size,f);
    fclose(f);

    std::string add=address;
    std::smatch match;
    std::regex reg("[a-zA-Z0-9\\s\\_\\-\\+]+[\\.]?[a-zA-Z0-9]+$");
    std::sregex_iterator curent(add.begin(),add.end(),reg);
    std::sregex_iterator end;

    match=*curent;
    std::string fileName;
    fileName=match.str();

    std::string fullName=direction;
    fullName+=fileName;
    char dest[fullName.length()];
    strcpy(dest,fullName.c_str());

    DIR *dir;
    dir=opendir(direction);
    if(dir==NULL){
        printf("Create directory %s\n",direction);
        mkdir(direction, 0700);
    }

    dir=opendir(direction);
    if(dir==NULL){
        printf("Could not create %s directory\n",direction);
        return;
    }

    time_t t;
    srand((unsigned) time(&t));
    int num=rand();
    std::string prefix=std::to_string(num);

    if((f=fopen(dest,"rb"))!=NULL){
        fclose(f);
        fullName=direction+prefix+"-"+fileName;
        char dest1[fullName.length()];
        strcpy(dest1,fullName.c_str());
        f=fopen(dest1,"wb");
    }else
        f=fopen(dest,"wb");

    fwrite(buf, sizeof(char),size, f);
    fclose(f);

    if(move){
        remove(address);
        printf("File %s moved to %s\n",address, direction);
    }else
        printf("File %s copied to %s\n",address, direction);
    
}

void no_name_excution(char *address, char *action, char *add){

    std::string ad=address;
    std::string addition=add;
    std::string s(ad);

    std::smatch match;
    std::regex reg("[a-zA-Z0-9\\s\\_\\-\\+]+[\\.]?[a-zA-Z0-9]+$");
    std::regex reg_ex("[\\.]?[a-zA-Z0-9]+$");

    std::sregex_iterator curent(s.begin(),s.end(),reg);
    std::sregex_iterator end;
    
    match=*curent;
    std::string fileName;
    fileName=match.str();

    std::smatch m;
    std::sregex_iterator cur(s.begin(),s.end(),reg_ex);
    std::sregex_iterator en;
    
    m=*cur;
    std::string extension;
    extension=m.str();
    std::string no_extension_filename= std::regex_replace(
        fileName,std::regex(extension),"");

    std::string folderName=std::regex_replace(
        ad,std::regex(fileName),"");

    if(strcmp(action,"-rp")==0){

        fileName=addition+fileName;
        std::string full=folderName+fileName;
        char fullName[full.length()];
        strcpy(fullName,full.c_str());

        if(rename(address,fullName)==0)
            printf("%s renamed to %s successfully\n",address,fullName);
        else
            printf("%s not renamed!\n",address);

    }else if(strcmp(action,"-rs")==0){

        std::string full=folderName+no_extension_filename+addition+extension;
        char fullName[full.length()];
        strcpy(fullName,full.c_str());

        if(rename(address,fullName)==0)
            printf("%s renamed to %s successfully\n",address,fullName);
        else
            printf("%s not renamed!\n",address);

    }else if(strcmp(action,"-rx")==0){

        std::string full=folderName+no_extension_filename+addition;
        char fullName[full.length()];
        strcpy(fullName,full.c_str());

        if(rename(address,fullName)==0)
            printf("%s renamed to %s successfully\n",address,fullName);
        else
            printf("%s not renamed!\n",address);

    }
}

void execution(char *address, char *selected){

    std::string add=address;
    std::string sel=selected;
    std::string s(add);

    std::smatch match;
    std::regex reg("[a-zA-Z0-9\\s\\_\\-\\+]+[\\.]?[a-zA-Z0-9]+$");

    std::sregex_iterator curent(s.begin(),s.end(),reg);
    std::sregex_iterator end;
    
    match=*curent;
    std::string fileName;
    fileName=match.str();

    std::string folderName=std::regex_replace(
        add,std::regex(fileName),"");

    int f_size=fileName.length();
    int d_size=folderName.length();

    char f_name[f_size];
    strcpy(f_name,fileName.c_str());
    char d_name[d_size];
    
    if(d_size!=0)
        strcpy(d_name,folderName.c_str());

    for(int i=0;i<e_count;i++){

        if(strcmp(exec[i].type,"-rn")==0){
            if(strcmp(exec[i].direction,"-rr")==0){

                char add[strlen(f_name)];
                strcpy(add,f_name);
                
                std::string s_add=add;
                s_add=std::regex_replace(s_add,std::regex(selected), exec[i].text);
                
                char rep[s_add.length()];
                strcpy(rep,s_add.c_str());

                int size=d_size+strlen(rep);

                char response[size];

                if(d_size!=0){
                    strcpy(response,d_name);
                    strcat(response,rep);
                }else
                    strcpy(response,rep);
                
                if(rename(address,response)==0)
                    printf("%s renamed to %s successfully\n",address,response);
                else
                    printf("%s not renamed!\n",address);
                
            }
        }
    }
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

bool testFile(char *str){

    bool _nc_rn_rr=false;
    bool _nr_rn_rr=false;

    char* _nc_rn_rr_text;
    char* _nr_rn_rr_text;

    FILE *f;
    f = fopen(str, "r");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fclose(f);

    bool result = true;
    for (int i = 0; i < c_count; i++){
        if (strstr("-sb", conn[i].type))
            if (conn[i].value > size)
                result = false;

        if (strstr("-ss", conn[i].type))
            if (result && conn[i].value < size)
                result = false;

        if (strstr("-se", conn[i].type))
            if (result && conn[i].value != size)
                result = false;

        if (strstr("-ex", conn[i].type))
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

        if (strstr("-nc", conn[i].type))
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
                            _nc_rn_rr=true;
                            _nc_rn_rr_text=c;
                            break;
                        }
                    }

                    result = sub;
                }else{
                    if (!strstr(str,conn[i].text))
                        result = false;
                    else{
                        _nc_rn_rr=true;
                        _nc_rn_rr_text=conn[i].text; 
                    }  
                }
            }

        if (strstr("-nr", conn[i].type)){
            
            std::string value(conn[i].text);
            std::string search(str);
            std::regex regex(value);
            std::smatch sm;
            std::regex_search(search,sm,regex);
            if(sm.length()>0){
                result= true;
                _nr_rn_rr=true;
                _nr_rn_rr_text=conn[i].text;
            }else
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
                std::string s=buf;
                std::string search(s);
                std::regex regex(value);
                std::smatch sm;

                std::sregex_iterator curent(s.begin(),s.end(),regex);
                std::sregex_iterator end;

                if(_lr){

                    int i=0;
                    while(curent!=end){
                        sm=*curent;

                        std::string smm=sm.str();
                        const char *ss= smm.c_str();

                        int size;
                        if(i==0){
                            size=strlen(ss)+4+strlen(str);
                        }else{
                            size=strlen(ss)+2;
                        }

                        if(size==2)
                            continue;

                        char match[size];

                        if(i==0){
                            strcpy(match,str);
                            strcat(match,":\n\t");
                            strcat(match,ss);
                            strcat(match,"\n");
                        }else{
                            strcpy(match,"\t");
                            strcat(match,ss);
                            strcat(match,"\n");
                        }
                        
                        int length=strlen(match);

                        if(load==NULL){
                            load=(char*)malloc(length);
                            int len=strlen(load);
                            memset(load,len,'\0');
                            strcpy(load,match);
                        }else{
                            load=(char*)realloc(load,strlen(load)+length*2);
                            strcat(load,match);
                        }
      
                        i++;
                        curent++;
                    }

                }else{

                    int i=0;
                    while(curent!=end){
                        i++;
                        curent++;
                    }
                        
                    if(i>0)
                        result= true;
                    else
                        result= false;

                }

            }else if(strcmp("-Cta", conn[i].type) == 0 || strcmp("-Ctc", conn[i].type) == 0){

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

    if(_nc_rn_rr && result)
        if(getFileNameContains(str, _nc_rn_rr_text))
            execution(str, _nc_rn_rr_text);

    if(_nr_rn_rr && result)
        if(getFileNameContainsRegex(str, _nr_rn_rr_text))
            execution(str, _nr_rn_rr_text);

    if(result){
        for(int i=0;i<e_count;i++)
            if(strcmp(exec[i].type,"-rn")==0){
                if(strcmp(exec[i].direction,"-rp")==0 || strcmp(exec[i].direction,"-rx")==0
                || strcmp(exec[i].direction,"-rs")==0)
                    no_name_excution(str, exec[i].direction, exec[i].text);
            }else if(strcmp(exec[i].type,"-mv")==0)
                moveCopy(str, exec[i].direction, true);
            else if(strcmp(exec[i].type,"-cp")==0)
                moveCopy(str, exec[i].direction, false);

        if(_r){
            remove(str);
            printf("%s removed\n",str);
        }

        if(_Rr){
            for(int o=0;o<e_count;o++)
                if(strcmp(exec[o].type,"-Rr")==0){

                    char *reg;
                    for(int o1=0;o1<c_count;o1++)
                        if(strstr(conn[o1].type,"-Ct"))
                            reg=conn[o1].text;

                    FILE *ff;
                    ff=fopen(str,"r");
                    fseek(ff,0,SEEK_END);
                    int ss=ftell(ff);
                    fseek(ff,0,SEEK_SET);
                    char b[ss];
                    fread(b,sizeof(char),ss,ff);
                    fclose(ff);

                    std::string s=b;
                    std::string c=exec[o].direction;
                    std::string sr=std::regex_replace(s,std::regex(reg),c);

                    char n_buf[sr.length()];
                    strcpy(n_buf,sr.c_str());

                    ff = fopen(str,"w");
                    fwrite(n_buf,sizeof(char),sr.length(),ff);
                    fclose(ff);
                    printf("Commited changes on file %s\n",str);

                }
        }

    }
    
    return result;
}