#ifndef PRINT_SUPPORT_H
#define PRINT_SUPPORT_H

#include <stdio.h>
#include <stdlib.h>

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

    printf("Suport options:\n\n");
    printf("\t-d\tinclude subdirectories in search.\n");
    printf("\t-D\tprovide directory path for execution. If not specified it's going to use...\n");
    printf("\t\t...root directory of the application.\n");
    printf("\t-l\tlist all selected files.\n");
    printf("\t-lr\t(works only with -Ctr)list all of matches found in file content per every selected file.\n\n");

    printf("Search options:\n\n");
    printf("\t-sb\tfiles bigger than this value in size expressed in bytes.\n");
    printf("\t-ss\tfiles smaller than this value in size expressed in bytes.\n");
    printf("\t-se\tfiles with this exact size expressed in bytes.\n");
    printf("\t-ex\tfiles with provided extension.\n");
    printf("\t-nc\tfiles with text contained in filename.\n");
    printf("\t-nr\tfiles with names matched with regular expression.\n");
    printf("\t-Cta\t(text files only)files that contain any of provided texts.\n");
    printf("\t\t...If you want to search for multiple sentences, separate them with |||\n");
    printf("\t-Ctc\t(text files only)files that contain all of provided texts.\n");
    printf("\t\t...If you want to search for multiple sentences, separate them with |||\n");
    printf("\t-Ctr\t(text files only)files that contain at least one match by regular expression.\n");
    printf("\t-Ctb\t(binary files only).files that contain provided hexadecimal pattern.\n\n");

    printf("Execute options(DANGEROUS):\n\n");
    printf("\t-cp\tCopy files to provided destination directory\n\n");
    printf("\t-mv\tmove selected files to specified directory. If destination directory already...\n");
    printf("\t\t...contains file name, new file name is going to have random number prefix attached...\n");
    printf("\t\t...If your file system doesn't contains destination directory\n");
    printf("\t\t..., File Scalper is going to create it.\n\n");
    printf("\t-rn\trename selected files. Provided support options:\n\n");
    printf("\t\t-rr\treplace search word in file names with provided name.\n");
    printf("\t\t\t...works only with -nc, -nr.\n");
    printf("\t\t-rp\tadd prefix to selected file name.\n");
    printf("\t\t-rs\tadd sufix to selected file name.\n");
    printf("\t\t-rx\tchange extension of selected files.\n\n");
    printf("\t-rm\tremove selected files.\n\n");
    printf("\t-Rr(works only with -Ctr) replace text matched with regular expression, with provided text.\n");
    printf("\t\t...can easily corrupt files.\n\n");

    printf("\n");
}

#endif