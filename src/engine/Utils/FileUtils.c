//
// Created by vd on 27.12.22.
//

#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>

// TODO: error handling

char* readFile(char* path){
    FILE* f = fopen(path, "r");
    if(f == NULL){
        fprintf(stderr,"File %s not found\n", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long fSize = ftell(f);
    rewind(f);

    char* ret = malloc(fSize + 1);
    if(ret == NULL){
        fprintf(stderr,"Failed to allocate memory to buffer for reading file %s\n", path);
        return NULL;
    }

    fread(ret, fSize, 1, f);
    fclose(f);
    ret[fSize] = 0;

    return ret;
}