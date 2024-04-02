#ifndef SPRITE_H
#define SPRITE_H

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

    const unsigned char *pallette;

    const unsigned char *data;

    unsigned short int bytesPerWidth;
    unsigned short int lines;
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

#endif