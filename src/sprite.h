#ifndef SPRITE_H
#define SPRITE_H
#include "utils.h"

enum MTLDSpriteFlags
{
    MTLDSpriteMirrorH = 1,
    MTLDSpriteMirrorV = 2,
    MTLDSpriteDoubleSize = 4
};

enum MTLDSpriteType
{
    MTLDSpritePallete,
    MTLDSpriteBitMaskColor,
};

struct MTLDSpriteBase
{
    unsigned char type;
    unsigned char flags;
    unsigned char r1;
    unsigned char r2;

    short int x;
    short int y;

    unsigned short int height;
    unsigned short int bytesPerLine;
};

struct MTLDSpritePallete
{
    struct MTLDSpriteBase shared;

    const unsigned short int *pallette;

    const unsigned char *data;

    unsigned short int bytesPerWidth;
    unsigned short int lines;

    char colorsPerByte;
    char n1, n2, n3;
};

struct MTLDSpriteBitMaskColor
{
    struct MTLDSpriteBase shared;

    unsigned int color;

    const unsigned char *data;

    unsigned short int bytesPerWidth;
    unsigned short int lines;
};

union MTLDSprite
{
    struct MTLDSpriteBase base;
    struct MTLDSpritePallete spritePallete;
    struct MTLDSpriteBitMaskColor spriteBitMaskColor;
};

EXPORT inline void mtldDrawSpritePallete(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer);
EXPORT inline void mtldDrawSpriteBitMaskColor(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer);

#endif