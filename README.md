First name of this project was Small Files Remover because I used it for purpose to scan huge file list based on parameter of file size and to remove all off files smaller than limit. Later on I realized that this app could be upgradet to do much more... so much more.

Purpose: scan through file system for files based on some parameter and execude command to rename, move, remove, edit or simple list selected files. What this app solves? It automatize process of search and execute on huge number of files.

How to compile: 

        g++ main.cpp -o fscalper


File Scalper is built for routine and fast tasks on large number of files.
Per every execution it initializes two processes. First process finds all files based on given parameters.
Second process executes command you have specified, on selected files.

        -h      prints this help.
        -help   same as -t.

Suport options:

        -d      include subdirectories in search.
        -l      list all selected files.

Search options:

        -sb     files bigger than this value in size expressed in bytes.
        -ss     files smaller than this value in size expressed in bytes.
        -se     files with this exact size expressed in bytes.
        -ex     files with extension expressed without dot.
        -nc     files with text contained in filename.
        -nr     files with names matched with regular expression.
        -Cta    (text files only)files that contain any of provided texts.
                If you want to search for multiple sentences, separate them with |||
        -Ctc    (text files only)files that contain all of provided texts.
                If you want to search for multiple sentences, separate them with |||
        -Ctr    (text files only)files that contain at least one match by regular expression.
        -lr     (works only with -Ctr)list all of matches found in file content per
                every selected file.
        -Ctb    (binary files only).files that contain provided hexadecimal pattern.