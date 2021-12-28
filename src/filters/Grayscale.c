#include "Grayscale.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);

Filter Grayscale = {
    .filterId = FILTER_GRAYSCALE,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    configurations->targetImageData->channels = 1;

    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;

    for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
    {
        int pixelIndex = index;
        targetData[pixelIndex] = (sourceData[pixelIndex * 3 + 0] + sourceData[pixelIndex * 3 + 1] + sourceData[pixelIndex * 3 + 2]) / 3;
    }

    return 0;
}