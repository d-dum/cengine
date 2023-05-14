//
// Created by vd on 27.12.22.
//

#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>




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


BMPImage* readBMP(char* path){
    FILE* file = fopen(path, "rb");
    if(!file){
        fprintf(stderr, "BMP file %s doesn't exist\n", path);
        return NULL;
    }

    unsigned char header[54];
    if(fread(header, 1, 54, file) != 54){
        fprintf(stderr, "Not a correct BMP file\n");
        return NULL;
    }

    if(header[0] != 'B' || header[1] != 'M'){
        fprintf(stderr, "Not a correct BMP file\n");
        return NULL;
    }

    BMPImage* image = (BMPImage*) malloc(sizeof(BMPImage));

    image->dataPos = *(int*)&(header[0x0A]);
    image->imageSize = *(int*)&(header[0x22]);
    image->width = *(int*)&(header[0x12]);
    image->height = *(int*)&(header[0x16]);

    if(image->imageSize == 0) image->imageSize = image->width * image->height * 3;
    if(image->dataPos == 0) image->dataPos = 54;

    image->data = (unsigned char*) malloc(sizeof(char)*image->imageSize);

    fread(image->data, 1, image->imageSize, file);
    fclose(file);

    return image;
}

void freeBMP(BMPImage* image){
    free(image->data);
    free(image);
}

//PNGImage* readPNG(char* path){
//    int width, height, channels;
//
//    PNGImage* img = (PNGImage*) malloc(sizeof(PNGImage));
//
//    img->data = stbi_load(path, &width, &height, &channels, 0);
//
//    if(!img->data){
//        free(img);
//        return NULL;
//    }
//
//    img->imageSize = width * height * channels;
//    img->height = height;
//    img->width = width;
//
//    return img;
//}

void freePNG(PNGImage* image){
    free(image->data);
    free(image);
}

