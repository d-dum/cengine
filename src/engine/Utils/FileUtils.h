//
// Created by vd on 27.12.22.
//

#ifndef ENGINE_FILEUTILS_H
#define ENGINE_FILEUTILS_H

typedef struct{
    unsigned int dataPos;
    unsigned int width;
    unsigned int height;
    unsigned int imageSize;
    unsigned char* data;
} BMPImage;

typedef struct{
    unsigned int width;
    unsigned int height;
    unsigned int imageSize;
    unsigned char* data;
} PNGImage;

char* readFile(char* path);
BMPImage* readBMP(char* path);
void freeBMP(BMPImage* image);
void freePNG(PNGImage* image);
PNGImage* readPNG(char* path);

#endif //ENGINE_FILEUTILS_H
