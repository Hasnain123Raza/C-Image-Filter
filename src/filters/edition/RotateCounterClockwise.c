#include "RotateCounterClockwise.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);

Filter RotateCounterClockwise = {
    .filterId = FILTER_ROTATE_COUNTERCLOCKWISE,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    configurations->targetImageData->width = sourceImageData->height;
    configurations->targetImageData->height = sourceImageData->width;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    Chunk *chunk = arguments->chunk;

    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;
    int channels = chunk->sourceImageData->channels;

    for (int pixelIndex = chunk->startPixelIndex; pixelIndex < chunk->endPixelIndex; pixelIndex++)
    {
        int sy = pixelIndex / chunk->sourceImageData->width;
        int sx = pixelIndex % chunk->sourceImageData->width;

        int ty = chunk->sourceImageData->width - sx - 1;
        int tx = sy;

        int targetPixelIndex = ty * chunk->targetImageData->width + tx;
        targetData[targetPixelIndex * 3 + 0] = sourceData[pixelIndex * 3 + 0];
        if (channels > 1)
            targetData[targetPixelIndex * 3 + 1] = sourceData[pixelIndex * 3 + 1];
        if (channels > 2)
            targetData[targetPixelIndex * 3 + 2] = sourceData[pixelIndex * 3 + 2];
        if (channels > 3)
            targetData[targetPixelIndex * 3 + 3] = sourceData[pixelIndex * 3 + 3];
    }

    return 0;
}