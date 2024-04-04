#ifndef MATILDA_H
#define MATILDA_H
#include "utils.h"
#include "sprite.h"

typedef void (*MTLDLineDrawHandler)(unsigned char *data, unsigned short int pixelsInLine, unsigned short int lineNumber);

struct MTLDContext
{
    union MTLDSprite *bufferDraw;

    union MTLDSprite *bufferBack;

    MTLDLineDrawHandler beforeLineDraw;
    MTLDLineDrawHandler afterLineDraw;

    unsigned short int maxSprites;
    unsigned short int drawSprite;

    unsigned short int backSprite;
    unsigned short int reserved;
};

EXPORT void mtldInitDoubleBuffer(struct MTLDContext *context, unsigned char *memBuffer, int sizeInBytes);
EXPORT void mtldInitSingleBuffer(struct MTLDContext *context, unsigned char *memBuffer, int sizeInBytes);
EXPORT char mtldIsUsingDoubleBuffer(struct MTLDContext *context);
EXPORT char mtldIsUsingSingleBuffer(struct MTLDContext *context);
EXPORT void mtldSetBeforeLineDrawFunction(struct MTLDContext *context, MTLDLineDrawHandler handler);
EXPORT void mtldSetAfterLineDrawFunction(struct MTLDContext *context, MTLDLineDrawHandler handler);
EXPORT void mtldPrepareNewFrame(struct MTLDContext *context);
EXPORT void mtldDrawFromDrawBuffer(struct MTLDContext *context, short int lineNumber, short int lineSize, unsigned char *outBuffer);
EXPORT void mtldDrawFromBackBuffer(struct MTLDContext *context, short int lineNumber, short int lineSize, unsigned char *outBuffer);
EXPORT void mtldDrawFromBuffer(struct MTLDContext *context, union MTLDSprite *buffer, int spritesAmount, short int lineNumber, short int lineSize, unsigned char *outBuffer);


EXPORT void mtldAddSpritePallete(
    struct MTLDContext *context,
    const unsigned short int *pallete,
    const unsigned char *data,
    char colorsPerByte,
    short int bytesPerWidth,
    short int bytesPerLine,
    unsigned short int lines,
    short x,
    short y,
    unsigned short int flags);

EXPORT void mtldAddSpriteBitMask(
    struct MTLDContext *context,
    const unsigned int color,
    const unsigned char *data,
    short int bytesPerWidth,
    short int bytesPerLine,
    unsigned short int lines,
    short x,
    short y,
    unsigned short int flags);

#endif