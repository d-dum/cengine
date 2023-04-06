//
// Created by vd on 27.12.22.
//

#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>

#include <png.h>

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

PNGImage* readPNG(char* path){
    FILE* file = fopen(path, "rb");
    if(!file){
        printf("Error: failed to open file %s\n", path);
        return NULL;
    }

    png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!pngPtr){
        printf("Error: png_create_read_struct failed\n");
        fclose(file);
        return NULL;
    }

    png_infop infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) {
        printf("Error: png_create_info_struct failed\n");
        png_destroy_read_struct(&pngPtr, NULL, NULL);
        fclose(file);
        return NULL;
    }

    if (setjmp(png_jmpbuf(pngPtr))) {
        printf("Error: setjmp failed\n");
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
        fclose(file);
        return NULL;
    }

    png_init_io(pngPtr, file);
    png_read_info(pngPtr, infoPtr);

    png_byte colorType = png_get_color_type(pngPtr, infoPtr);
    png_byte bitDepth = png_get_bit_depth(pngPtr, infoPtr);

    if (bitDepth == 16) {
        png_set_strip_16(pngPtr);
    }

    if (colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(pngPtr);
    }

    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
        png_set_expand_gray_1_2_4_to_8(pngPtr);
    }

    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pngPtr);
    } else {
        png_set_filler(pngPtr, 0xFF, PNG_FILLER_AFTER);
    }

    png_read_update_info(pngPtr, infoPtr);

    png_uint_32 width = png_get_image_width(pngPtr, infoPtr);
    png_uint_32 height = png_get_image_height(pngPtr, infoPtr);
    png_byte channels = png_get_channels(pngPtr, infoPtr);

    PNGImage* image = malloc(sizeof(PNGImage));
    image->width = width;
    image->height = height;
    image->imageSize = width * height * channels;

    image->data = (unsigned char*)malloc(image->imageSize);

    png_bytep* rowPointers = malloc(sizeof(png_bytep) * height);

    for (unsigned int i = 0; i < height; ++i) {
        rowPointers[i] = image->data + i * width * channels;
    }

    png_read_image(pngPtr, rowPointers);

    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    fclose(file);

    return image;
}

void freePNG(PNGImage* image){
    free(image->data);
    free(image);
}