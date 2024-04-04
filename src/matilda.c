#include "matilda.h"

void mtldInitDoubleBuffer(struct MTLDContext *context, unsigned char *memBuffer, int sizeInBytes)
{
    int spriteSize = sizeof(union MTLDSprite);
    int spritesAmount = sizeInBytes / spriteSize;
    int spritesPerBuffer = spritesAmount / 2;

    context->beforeLineDraw = nullptr;
    context->afterLineDraw = nullptr;
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

    context->beforeLineDraw = nullptr;
    context->afterLineDraw = nullptr;
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

void mtldSetBeforeLineDrawFunction(struct MTLDContext *context, MTLDLineDrawHandler handler)
{
    context->beforeLineDraw = handler;
}

void mtldSetAfterLineDrawFunction(struct MTLDContext *context, MTLDLineDrawHandler handler)
{
    context->afterLineDraw = handler;
}

void mtldPrepareNewFrame(struct MTLDContext *context)
{
    union MTLDSprite *bufferSwap = context->bufferDraw;
    context->bufferDraw = context->bufferBack;
    context->bufferBack = bufferSwap;
    context->backSprite = context->drawSprite;
    context->drawSprite = 0;
}

void mtldDrawFromDrawBuffer(struct MTLDContext *context, short int lineNumber, short int lineSize, unsigned char *outBuffer)
{
    mtldDrawFromBuffer(context, context->bufferDraw, context->drawSprite, lineNumber, lineSize, outBuffer);
}

void mtldDrawFromBackBuffer(struct MTLDContext *context, short int lineNumber, short int lineSize, unsigned char *outBuffer)
{
    mtldDrawFromBuffer(context, context->bufferBack, context->backSprite, lineNumber, lineSize, outBuffer);
}

void mtldDrawFromBuffer(struct MTLDContext *context, union MTLDSprite *buffer, int spritesAmount, short int lineNumber, short int lineSize, unsigned char *outBuffer)
{
    if (context->beforeLineDraw)
    {
        context->beforeLineDraw(outBuffer, lineSize, lineNumber);
    }
    else
    {
        for (int i = 0; i < lineSize; i++)
        {
            ((unsigned short int *)outBuffer)[i] = 0;
        }
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

    if (context->afterLineDraw)
    {
        context->afterLineDraw(outBuffer, lineSize, lineNumber);
    }
}

void mtldAddSpritePallete(
    struct MTLDContext *context,
    const unsigned short int *pallete,
    const unsigned char *data,
    char colorsPerByte,
    short int bytesPerWidth,
    short int bytesPerLine,
    unsigned short int lines,
    short x,
    short y,
    unsigned short int flags)
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
        spriteData->spritePallete.colorsPerByte = colorsPerByte;
    }
}

void mtldAddSpriteBitMask(
    struct MTLDContext *context,
    const unsigned int color,
    const unsigned char *data,
    short int bytesPerWidth,
    short int bytesPerLine,
    unsigned short int lines,
    short x,
    short y,
    unsigned short int flags)
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
