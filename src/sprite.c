#include "sprite.h"

inline void mtldDrawSpritePallete(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer)
{
    char mirrorH = sprite->base.flags & MTLDSpriteMirrorH;
    char mirrorV = sprite->base.flags & MTLDSpriteMirrorV;

    unsigned short int line = mirrorV ? sprite->base.height - (lineNumber - sprite->base.y) - 1 : lineNumber - sprite->base.y;
    const unsigned char *lineData = sprite->spritePallete.data + line * sprite->base.bytesPerLine;
    short int x = sprite->base.x;
    unsigned char c;

    if (sprite->spritePallete.colorsPerByte == 1)
    {
        if (mirrorH)
            for (int i = 0; i < sprite->spritePallete.bytesPerWidth; i++)
            {
                short int shift = x + sprite->spritePallete.bytesPerWidth - i - 1;
                if (shift >= 0 && shift < 320)
                    outBuffer[shift] = sprite->spritePallete.pallette[lineData[i]];
            }
        else
            for (int i = 0; i < sprite->spritePallete.bytesPerWidth; i++)
            {
                short int shift = x + i;
                if (shift >= 0 && shift < 320)
                    outBuffer[shift] = sprite->spritePallete.pallette[lineData[i]];
            }
    }
    else if (sprite->spritePallete.colorsPerByte == 2)
    {
        if (mirrorH)
            for (int i = 0; i < sprite->spritePallete.bytesPerWidth; i++)
            {
                short int shift = x + ((sprite->spritePallete.bytesPerWidth - i - 1) << 1);
                c = (lineData[i] >> 4) & 0x0f;
                if (c && shift >= -1 && shift < 319)
                    outBuffer[shift + 1] = sprite->spritePallete.pallette[c];
                c = (lineData[i]) & 0x0f;
                if (c && shift >= 0 && shift < 320)
                    outBuffer[shift] = sprite->spritePallete.pallette[c];
            }
        else
            for (int i = 0; i < sprite->spritePallete.bytesPerWidth; i++)
            {
                short int shift = x + (i << 1);
                c = (lineData[i] >> 4) & 0x0f;
                if (c && shift >= 0 && shift < 320)
                    outBuffer[shift] = sprite->spritePallete.pallette[c];
                c = (lineData[i]) & 0x0f;
                if (c && shift >= -1 && shift < 319)
                    outBuffer[shift + 1] = sprite->spritePallete.pallette[c];
            }
    }
    else if (sprite->spritePallete.colorsPerByte == 4)
    {
        if (mirrorH)
            for (int i = 0; i < sprite->spritePallete.bytesPerWidth; i++)
            {
                short int shift = x + ((sprite->spritePallete.bytesPerWidth - i - 1) << 2);
                c = (lineData[i] >> 6) & 0x03;
                if (c && shift >= -3 && shift < 317)
                    outBuffer[shift + 3] = sprite->spritePallete.pallette[c];
                c = (lineData[i] >> 4) & 0x03;
                if (c && shift >= -2 && shift < 318)
                    outBuffer[shift + 2] = sprite->spritePallete.pallette[c];
                c = (lineData[i] >> 2) & 0x03;
                if (c && shift >= -1 && shift < 319)
                    outBuffer[shift + 1] = sprite->spritePallete.pallette[c];
                c = (lineData[i]) & 0x03;
                if (c && shift >= 0 && shift < 320)
                    outBuffer[shift] = sprite->spritePallete.pallette[c];
            }
        else
            for (int i = 0; i < sprite->spritePallete.bytesPerWidth; i++)
            {
                short int shift = x + (i << 2);
                c = (lineData[i] >> 6) & 0x03;
                if (c)
                    outBuffer[shift] = sprite->spritePallete.pallette[c];
                c = (lineData[i] >> 4) & 0x03;
                if (c)
                    outBuffer[shift + 1] = sprite->spritePallete.pallette[c];
                c = (lineData[i] >> 2) & 0x03;
                if (c)
                    outBuffer[shift + 2] = sprite->spritePallete.pallette[c];
                c = (lineData[i]) & 0x03;
                if (c)
                    outBuffer[shift + 3] = sprite->spritePallete.pallette[c];
            }
    }
}

inline void mtldDrawSpriteBitMaskColor(union MTLDSprite *sprite, short int lineNumber, unsigned short int *outBuffer)
{
    char mirrorH = sprite->base.flags & MTLDSpriteMirrorH;
    char mirrorV = sprite->base.flags & MTLDSpriteMirrorV;

    unsigned short int line = mirrorV ? sprite->base.height - (lineNumber - sprite->base.y) - 1 : lineNumber - sprite->base.y;
    const unsigned char *lineData = sprite->spriteBitMaskColor.data + line * sprite->base.bytesPerLine;
    unsigned short int color = sprite->spriteBitMaskColor.color & 0xffff;
    short int x = sprite->base.x;

    if (mirrorH)
        for (int i = 0; i < sprite->spriteBitMaskColor.bytesPerWidth; i++)
        {
            short int shift = x + ((sprite->spriteBitMaskColor.bytesPerWidth - i - 1) << 3);
            if ((lineData[i] & 0b00000001) && shift >= 0 && shift < 320)
                outBuffer[shift + 0] = color;
            if ((lineData[i] & 0b00000010) && shift >= -1 && shift < 319)
                outBuffer[shift + 1] = color;
            if ((lineData[i] & 0b00000100) && shift >= -2 && shift < 318)
                outBuffer[shift + 2] = color;
            if ((lineData[i] & 0b00001000) && shift >= -3 && shift < 317)
                outBuffer[shift + 3] = color;
            if ((lineData[i] & 0b00010000) && shift >= -4 && shift < 316)
                outBuffer[shift + 4] = color;
            if ((lineData[i] & 0b00100000) && shift >= -5 && shift < 315)
                outBuffer[shift + 5] = color;
            if ((lineData[i] & 0b01000000) && shift >= -6 && shift < 314)
                outBuffer[shift + 6] = color;
            if ((lineData[i] & 0b10000000) && shift >= -7 && shift < 313)
                outBuffer[shift + 7] = color;
        }
    else
        for (int i = 0; i < sprite->spriteBitMaskColor.bytesPerWidth; i++)
        {
            short int shift = x + (i << 3);
            if ((lineData[i] & 0b00000001) && shift >= -7 && shift < 313)
                outBuffer[shift + 7] = color;
            if ((lineData[i] & 0b00000010) && shift >= -6 && shift < 314)
                outBuffer[shift + 6] = color;
            if ((lineData[i] & 0b00000100) && shift >= -5 && shift < 315)
                outBuffer[shift + 5] = color;
            if ((lineData[i] & 0b00001000) && shift >= -4 && shift < 316)
                outBuffer[shift + 4] = color;
            if ((lineData[i] & 0b00010000) && shift >= -3 && shift < 317)
                outBuffer[shift + 3] = color;
            if ((lineData[i] & 0b00100000) && shift >= -2 && shift < 318)
                outBuffer[shift + 2] = color;
            if ((lineData[i] & 0b01000000) && shift >= -1 && shift < 319)
                outBuffer[shift + 1] = color;
            if ((lineData[i] & 0b10000000) && shift >= 0 && shift < 320)
                outBuffer[shift + 0] = color;
        }
}