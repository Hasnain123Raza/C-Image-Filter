#include "Saturation.h"

typedef struct {
    float saturation;
} SaturationUserData;

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);
static void FreeUserData(void *userData);

Filter Saturation = {
    .filterId = FILTER_SATURATION,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    if (sourceImageData->channels < 3)
        return 1;

    SaturationUserData *userData = (SaturationUserData *)malloc(sizeof(SaturationUserData));
    if (!userData)
        return 1;
    
    char *saturation = argz_next(filterRequest->arguments->value, filterRequest->arguments->length, NULL);
    userData->saturation = saturation ? atof(saturation) : 0;

    configurations->userData = userData;
    configurations->freeUserData = FreeUserData;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    Chunk *chunk = arguments->chunk;
    void *userData = arguments->userData;

    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;
    int channels = chunk->sourceImageData->channels;

    float saturation = ((SaturationUserData *)userData)->saturation;

    for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
    {
        int sourceRed = sourceData[index * 3 + 0] + 1;
        int sourceGreen = sourceData[index * 3 + 1] + 1;
        int sourceBlue = sourceData[index * 3 + 2] + 1;

        int average = (sourceRed + sourceGreen + sourceBlue) / 3;

        sourceRed = average + (sourceRed - average) * saturation;
        sourceGreen = average + (sourceGreen - average) * saturation;
        sourceBlue = average + (sourceBlue - average) * saturation;

        if (sourceRed > 255)
            sourceRed = 255;
        if (sourceRed < 0)
            sourceRed = 0;

        if (sourceGreen > 255)
            sourceGreen = 255;
        if (sourceGreen < 0)
            sourceGreen = 0;

        if (sourceBlue > 255)
            sourceBlue = 255;
        if (sourceBlue < 0)
            sourceBlue = 0;

        targetData[index * channels + 0] = (unsigned char)sourceRed;
        targetData[index * channels + 1] = (unsigned char)sourceGreen;
        targetData[index * channels + 2] = (unsigned char)sourceBlue;
    }

    if (channels == 4)
    {
        for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
            targetData[index * channels + 3] = sourceData[index * channels + 3];
    }

    return 0;
}

static void FreeUserData(void *userData)
{
    free(userData);
}