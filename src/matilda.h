#ifndef MATILDA_H
#define MATILDA_H
#include "utils.h"
#include "sprite.h"

enum MTLD_MODE
{
    MTLD_MODE_565,
    MTLD_MODE_8888
};

struct MTLDContext
{
    union MTLDSprite *bufferDraw;

    union MTLDSprite *bufferBack;

    unsigned short int maxSprites;
    unsigned short int drawSprite;

    unsigned short int backSprite;
    unsigned short int reserved;
};

EXPORT void mtldInitDoubleBuffer(struct MTLDContext *context, unsigned char *memBuffer, int sizeInBytes);
EXPORT void mtldInitSingleBuffer(struct MTLDContext *context, unsigned char *memBuffer, int sizeInBytes);
EXPORT char mtldIsUsingDoubleBuffer(struct MTLDContext *context);
EXPORT char mtldIsUsingSingleBuffer(struct MTLDContext *context);
EXPORT void mtldPrepareNewFrame(struct MTLDContext *context);
EXPORT int mtldCalcLineBufferSize(enum MTLD_MODE mode, int pixelsInLine);
EXPORT void mtldDrawFromDrawBuffer(struct MTLDContext *context, enum MTLD_MODE mode, short int lineNumber, short int lineSize, unsigned char *outBuffer);
EXPORT void mtldDrawFromBackBuffer(struct MTLDContext *context, enum MTLD_MODE mode, short int lineNumber, short int lineSize, unsigned char *outBuffer);
EXPORT void mtldDrawFromBuffer(struct MTLDContext *context, enum MTLD_MODE mode, union MTLDSprite *buffer, int spritesAmount, short int lineNumber, short int lineSize, unsigned char *outBuffer);

EXPORT inline void mtldDrawSpritePallete(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer);
EXPORT inline void mtldDrawSpriteBitMaskColor(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer);

EXPORT void mtldAddSpritePallete(
    struct MTLDContext *context,
    const unsigned char *pallete,
    const unsigned char *data,
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