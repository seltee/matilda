#include "matilda.h"

void mtldInitDoubleBuffer(struct MTLDContext *context, unsigned char *memBuffer, int sizeInBytes)
{
    int spriteSize = sizeof(union MTLDSprite);
    int spritesAmount = sizeInBytes / spriteSize;
    int spritesPerBuffer = spritesAmount / 2;

    context->bufferDraw = (union MTLDSprite *)memBuffer;
    context->bufferBack = &context->bufferDraw[spritesPerBuffer];
    context->maxSprites = spritesPerBuffer;
    context->drawSprite = 0;
    context->backSprite = 0;
}

void mtldInitSingleBuffer(struct MTLDContext *context, unsigned char *memBuffer, int sizeInBytes)
{
    int spriteSize = sizeof(union MTLDSprite);
    int spritesAmount = sizeInBytes / spriteSize;

    context->bufferDraw = (union MTLDSprite *)memBuffer;
    context->bufferBack = context->bufferDraw;
    context->maxSprites = spritesAmount;
    context->drawSprite = 0;
    context->backSprite = 0;
}

char mtldIsUsingDoubleBuffer(struct MTLDContext *context)
{
    return (context->bufferDraw != context->bufferBack && context->bufferDraw != 0);
}

char mtldIsUsingSingleBuffer(struct MTLDContext *context)
{
    return (context->bufferDraw == context->bufferBack && context->bufferDraw != 0);
}

void mtldPrepareNewFrame(struct MTLDContext *context)
{
    union MTLDSprite *bufferSwap = context->bufferDraw;
    context->bufferDraw = context->bufferBack;
    context->bufferBack = bufferSwap;
    context->backSprite = context->drawSprite;
    context->drawSprite = 0;
}

int mtldCalcLineBufferSize(enum MTLD_MODE mode, int pixelsInLine)
{
    if (mode == MTLD_MODE_565)
    {
        return 2 * pixelsInLine;
    }
    if (mode == MTLD_MODE_8888)
    {
        return 4 * pixelsInLine;
    }
    return 0;
}

void mtldDrawFromDrawBuffer(struct MTLDContext *context, enum MTLD_MODE mode, short int lineNumber, short int lineSize, unsigned char *outBuffer)
{
    mtldDrawFromBuffer(context, mode, context->bufferDraw, context->drawSprite, lineNumber, lineSize, outBuffer);
}

void mtldDrawFromBackBuffer(struct MTLDContext *context, enum MTLD_MODE mode, short int lineNumber, short int lineSize, unsigned char *outBuffer)
{
    mtldDrawFromBuffer(context, mode, context->bufferBack, context->backSprite, lineNumber, lineSize, outBuffer);
}

inline void mtldDrawSpritePallete(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer)
{
    unsigned short int line = lineNumber - sprite->base.y;
    const unsigned char *lineData = sprite->spritePallete.data + line * sprite->base.bytesPerLine;
    short int x = sprite->base.x;

    for (int i = 0; i < sprite->spritePallete.bytesPerWidth; i++)
    {
        short int shift = x + i;
        unsigned short int color = sprite->spritePallete.pallette[(lineData[i] << 1)] + (sprite->spritePallete.pallette[(lineData[i] << 1) + 1] << 8);
        outBuffer[shift] = color;
    }
}

inline void mtldDrawSpriteBitMaskColor(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer)
{
    unsigned short int line = lineNumber - sprite->base.y;
    const unsigned char *lineData = sprite->spriteBitMaskColor.data + line * sprite->base.bytesPerLine;
    unsigned short int color = sprite->spriteBitMaskColor.color & 0xffff;
    short int x = sprite->base.x;

    for (int i = 0; i < sprite->spriteBitMaskColor.bytesPerWidth; i++)
    {
        short int shift = x + (i << 3);
        if (lineData[i] & 0b00000001)
            outBuffer[shift + 7] = color;
        if (lineData[i] & 0b00000010)
            outBuffer[shift + 6] = color;
        if (lineData[i] & 0b00000100)
            outBuffer[shift + 5] = color;
        if (lineData[i] & 0b00001000)
            outBuffer[shift + 4] = color;
        if (lineData[i] & 0b00010000)
            outBuffer[shift + 3] = color;
        if (lineData[i] & 0b00100000)
            outBuffer[shift + 2] = color;
        if (lineData[i] & 0b01000000)
            outBuffer[shift + 1] = color;
        if (lineData[i] & 0b10000000)
            outBuffer[shift + 0] = color;
    }
}

void mtldDrawFromBuffer(struct MTLDContext *context, enum MTLD_MODE mode, union MTLDSprite *buffer, int spritesAmount, short int lineNumber, short int lineSize, unsigned char *outBuffer)
{
    for (int i = 0; i < lineSize; i++)
    {
        outBuffer[i * 2 + 1] = 0;
        outBuffer[i * 2] = 0;
    }

    for (int i = 0; i < spritesAmount; i++)
    {
        union MTLDSprite *sprite = &buffer[i];
        if (lineNumber >= sprite->base.y && lineNumber < sprite->base.y + sprite->base.height)
        {
            switch (sprite->base.type)
            {
            case MTLDSpritePallete:
                mtldDrawSpritePallete(sprite, lineNumber, (unsigned short int *)outBuffer);
                break;

            case MTLDSpriteBitMaskColor:
                mtldDrawSpriteBitMaskColor(sprite, lineNumber, (unsigned short int *)outBuffer);
                break;

            default:
                break;
            }
        }
    }
}

void mtldAddSpritePallete(struct MTLDContext *context, const unsigned char *pallete, const unsigned char *data, short int bytesPerWidth, short int bytesPerLine, unsigned short int lines, short x, short y, unsigned short int flags)
{
    if (context->drawSprite < context->maxSprites)
    {
        union MTLDSprite *spriteData = &context->bufferDraw[context->drawSprite];
        context->drawSprite++;

        spriteData->base.type = MTLDSpritePallete;
        spriteData->base.flags = flags;
        spriteData->base.x = x;
        spriteData->base.y = y;
        spriteData->base.height = lines;
        spriteData->base.bytesPerLine = bytesPerLine;
        spriteData->spritePallete.pallette = pallete;
        spriteData->spritePallete.data = data;
        spriteData->spritePallete.bytesPerWidth = bytesPerWidth;
        spriteData->spritePallete.lines = lines;
    }
}

void mtldAddSpriteBitMask(struct MTLDContext *context, const unsigned int color, const unsigned char *data, short int bytesPerWidth, short int bytesPerLine, unsigned short int lines, short x, short y, unsigned short int flags)
{
    if (context->drawSprite < context->maxSprites)
    {
        union MTLDSprite *spriteData = &context->bufferDraw[context->drawSprite];
        context->drawSprite++;

        spriteData->base.type = MTLDSpriteBitMaskColor;
        spriteData->base.flags = flags;
        spriteData->base.x = x;
        spriteData->base.y = y;
        spriteData->base.height = lines;
        spriteData->base.bytesPerLine = bytesPerLine;
        spriteData->spriteBitMaskColor.color = color;
        spriteData->spriteBitMaskColor.data = data;
        spriteData->spriteBitMaskColor.bytesPerWidth = bytesPerWidth;
        spriteData->spriteBitMaskColor.lines = lines;
    }
}
