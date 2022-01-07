#include "Brightness.h"

typedef struct {
    int brightness;
} BrightnessUserData;

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);
static void FreeUserData(void *userData);

Filter Brightness = {
    .filterId = FILTER_BRIGHTNESS,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    BrightnessUserData *userData = (BrightnessUserData *)malloc(sizeof(BrightnessUserData));
    if (!userData)
        return 1;
    
    char *brightness = argz_next(filterRequest->arguments->value, filterRequest->arguments->length, NULL);
    userData->brightness = brightness ? atoi(brightness) : 0;

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

    int brightness = ((BrightnessUserData *)userData)->brightness;

    if (channels == 1 || channels == 2)
    {
        for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
        {
            int sourceGray = sourceData[index * channels + 0] + brightness;

            if (sourceGray > 255)
                sourceGray = 255;
            if (sourceGray < 0)
                sourceGray = 0;

            targetData[index * channels + 0] = (unsigned char)sourceGray;
        }

        if (channels == 2)
        {
            for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
                targetData[index * channels + 1] = sourceData[index * channels + 1];
        }
    }
    else
    {
        for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
        {
            int sourceRed = sourceData[index * channels + 0] + brightness;
            int sourceGreen = sourceData[index * channels + 1] + brightness;
            int sourceBlue = sourceData[index * channels + 2] + brightness;

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
    }

    return 0;
}

static void FreeUserData(void *userData)
{
    free(userData);
}