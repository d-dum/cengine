#include "Label.h"
#include "../../../lib/freetype/include/ft2build.h"
#include FT_FREETYPE_H
#include <stdlib.h>
#include <stdio.h>

void printCharacter(Character* c, char i){
    printf("Character pos %c textureId %d width %d\n", i, c->textureID, c->size_x);
}

Character** loadFont(char* path, int pixelSize){
    FT_Library ft;
    if(FT_Init_FreeType(&ft)){
        fprintf(stderr, "ERROR::FREETYPE: Could not init FreeType Library\n");
        return NULL;
    }

    FT_Face face;
    if(FT_New_Face(ft, path, 0, &face)){
        fprintf(stderr, "ERROR::FREETYPE: Failed to load font\n");
        return NULL;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    if(FT_Load_Char(face, 'X', FT_LOAD_RENDER)){
        fprintf(stderr, "ERROR::FREETYTPE: Failed to load Glyph\n");
        return NULL;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Character** charSet = (Character**) malloc(sizeof(Character*) * 128);

    for(unsigned char c = 0; c < 128; c++){
        Character* character = (Character*) malloc(sizeof(Character));
        int result = FT_Load_Char(face, c, FT_LOAD_RENDER);
        FT_GlyphSlot glyph = face->glyph;
        if (result){
            fprintf(stderr, "ERROR::FREETYTPE: Failed to load Glyph\n");
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            fprintf(stderr, "ERROR: Failed to generate texture\n");
            free(character);
            continue;
        }

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            glyph->bitmap.width,
            glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        character->textureID = texture;

        printf("TextureId: %d, character %c\n", texture, c);

        character->size_x = glyph->bitmap.width;
        character->size_y = glyph->bitmap.rows;

        character->bearing_x = glyph->bitmap_left;
        character->bearing_y = glyph->bitmap_top;
        character->advance = glyph->advance.x;

        charSet[c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return charSet;
}

void deleteCharSet(Character** charSet){
    for(int i = 0; i < 128; i++){
        free(charSet[i]);
    }
    free(charSet);
}