#include "Grayscale.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);

Filter Grayscale = {
    .filterId = FILTER_GRAYSCALE,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    if (sourceImageData->channels < 3)
        return 1;
    
    configurations->targetImageData->channels = sourceImageData->channels == 3 ? 1 : 2;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    Chunk *chunk = arguments->chunk;

    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;
    int channels = chunk->sourceImageData->channels;

    for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
        targetData[index] = (sourceData[index * channels + 0] + sourceData[index * channels + 1] + sourceData[index * channels + 2]) / 3;
    

    if (channels == 4)
    {
        for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
            targetData[index * channels + 3] = sourceData[index * channels + 3];
    }

    return 0;
}