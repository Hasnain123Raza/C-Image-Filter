#include "FlipHorizontal.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);

Filter FlipHorizontal = {
    .filterId = FILTER_FLIP_HORIZONTAL,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;

    for (int pixelIndex = chunk->startPixelIndex; pixelIndex < chunk->endPixelIndex; pixelIndex++)
    {
        int sy = pixelIndex / chunk->sourceImageData->width;
        int sx = pixelIndex % chunk->sourceImageData->width;

        int ty = sy;
        int tx = chunk->sourceImageData->width - sx - 1;

        int targetPixelIndex = ty * chunk->targetImageData->width + tx;
        targetData[targetPixelIndex * 3 + 0] = sourceData[pixelIndex * 3 + 0];
        targetData[targetPixelIndex * 3 + 1] = sourceData[pixelIndex * 3 + 1];
        targetData[targetPixelIndex * 3 + 2] = sourceData[pixelIndex * 3 + 2];
    }

    return 0;
}