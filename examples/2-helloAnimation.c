#include "matilda.h"
#include "helpers.h"
#include "rrh.h"

int counter = 0;

void updateLoop(struct MTLDContext *context)
{
    mtldPrepareNewFrame(context);
    counter++;
    int frame = ((counter / 4) % 12);
    int x = frame % 3;
    int y = frame / 3;
    mtldAddSpritePallete(context, pallete, image_rrh + x * 20 + y * 60 * 40, 2, 20, 60, 40, ((counter * 2) % 400) - 40, 200, MTLDSpriteMirrorH | MTLDSpriteDoubleSize);
}

void beforeRender(unsigned char *data, unsigned short int pixelsInLine, unsigned short int lineNumber)
{
    unsigned short *pixels = (unsigned short *)data;
    unsigned short int color = 0x7370 + ((lineNumber / 16) << 11) + (lineNumber / 32);
    for (int i = 0; i < pixelsInLine; i++)
    {
        pixels[i] = color;
    }
}

int main()
{
    unsigned char spriteMem[4096];
    struct MTLDContext context;
    mtldInitDoubleBuffer(&context, spriteMem, 4096);
    mtldSetBeforeLineDrawFunction(&context, beforeRender);

    makeWindow(GetModuleHandle(nullptr), 1, updateLoop, &context, "Example 1: Hello World");

    return runGameLoop();
}
