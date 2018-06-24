//
//  main.cpp
//  Lab5
//
//  Created by Borzoo Bonakdarpour on 2016-03-29.
//  Copyright © 2016 Borzoo Bonakdarpour. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <time.h>
#include <dirent.h>

using namespace std;

void PrintAttributes(struct stat *st){
    cout << "Mode: " << oct << st -> st_mode << endl;
    cout << "Number of links: " << st -> st_nlink << endl;
    cout << "Owner: " << getpwuid(st -> st_uid)->pw_name << endl;
    cout << "Group: " << getgrgid(st -> st_gid)->gr_name << endl;
    printf("File size: %lld byte(s)\n", (long long) st->st_size);
    cout << "Size: " << (st -> st_size / (int)st -> st_blksize) + 1 << " block(s)" << endl;
    
    struct tm *tmp;
    char outstr[200];
    
    tmp = localtime(&st -> st_mtime);
    if (tmp == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }
    strftime(outstr, sizeof(outstr), "%d %b %Y, %H:%M", tmp);
    cout << "Last modification time: " << outstr << endl;
}

void Traverse(char* name){
    struct dirent *de;
    struct stat buf;
    int exists;

    DIR *d;
    d = opendir(name);
    if (d == NULL) {
        perror("prsize");
        return;
    }
    // cout << "This is a directory" << endl;
    
    for (de = readdir(d); de != NULL; de = readdir(d)) {
        char str[1024] = "";
        strcpy(str, name);
        strcat(str, "/");
        strcat(str, de ->d_name);
        exists = stat(str, &buf);
        cout << de -> d_name << endl;
        
        if (S_ISDIR(buf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            Traverse((char*)(str));
        }

    }
    closedir(d);
}


int main(int argc, const char** argv) {

    struct stat st;
    
    if (argc != 2){
        cout << "Enter a file or directory name." << endl;
        return 0;
    }

    if (0 != stat(argv[1], &st)) {
        perror("fstat");
        return 0;
    }
    
    if ((st.st_mode & S_IFMT) == S_IFREG){
        cout << "This is a regular file:" << endl << endl;
        PrintAttributes(&st);
        return 1;
    }
    

    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        cout << "This is a directory" << endl;
        Traverse((char*)argv[1]); }
    
    return 1;
}
