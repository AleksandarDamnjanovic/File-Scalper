# File Scalper

First name of this project was Small Files Remover because I used it for purpose to scan huge file list based on parameter of file size and to remove all off files smaller than limit. Later on I realized that this app could be upgradet to do much more... so much more.

Purpose: scan through file system for files based on some parameter and execude command to rename, move, remove, edit or simple list selected files. What this app solves? It automatize process of search and execute on huge number of files.

### How to compile: 

        g++ -g main.cpp print_support.h -o run.bin


File Scalper is built for routine and fast tasks on large number of files.
Per every execution it initializes two processes. First process finds all files based on given parameters.
Second process executes command you have specified, on selected files.

        -h      prints this help.
        -help   same as -t.

### Suport options:

        -d      include subdirectories in search.
        -D      provide directory path for execution. If not specified it's going to use...
                ...root directory of the application.
        -l      list all selected files.
        -lr     (works only with -Ctr)list all of matches found in file content per every selected file.

### Search options:

        -sb     files bigger than this value in size expressed in bytes.
        -ss     files smaller than this value in size expressed in bytes.
        -se     files with this exact size expressed in bytes.
        -ex     files with provided extension.
        -nc     files with text contained in filename.
        -nr     files with names matched with regular expression.
        -Cta    (text files only)files that contain any of provided texts.
                ...If you want to search for multiple sentences, separate them with |||
        -Ctc    (text files only)files that contain all of provided texts.
                ...If you want to search for multiple sentences, separate them with |||
        -Ctr    (text files only)files that contain at least one match by regular expression.
        -Ctb    (binary files only).files that contain provided hexadecimal pattern.

### Execute options(DANGEROUS):

        -cp     Copy files to provided destination directory

        -mv     move selected files to specified directory. If destination directory already...
                ...contains file name, new file name is going to have random number prefix attached...
                ...If your file system doesn't contains destination directory
                ..., File Scalper is going to create it.

        -rn     rename selected files. Provided support options:

                -rr     replace search word in file names with provided name.
                        ...works only with -nc, -nr.
                -rp     add prefix to selected file name.
                -rs     add sufix to selected file name.
                -rx     change extension of selected files.

        -rm     remove selected files.

        -Rr(works only with -Ctr) replace text matched with regular expression, with provided text.
                ...can easily corrupt files.
