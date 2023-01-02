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

char* readFile(char* path);
BMPImage* readBMP(char* path);
void freeBMP(BMPImage* image);

#endif //ENGINE_FILEUTILS_H
