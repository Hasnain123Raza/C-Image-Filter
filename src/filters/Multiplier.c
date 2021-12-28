#include "Multiplier.h"

typedef struct {
    float multipliers[9];
} MultiplierUserData;

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);
static void FreeUserData(void *userData);

Filter Multiplier = {
    .filterId = FILTER_MULTIPLIER,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    MultiplierUserData *userData = (MultiplierUserData *)malloc(sizeof(MultiplierUserData));
    if (!userData)
        return 1;
    userData->multipliers[0] = 1.0f;
    userData->multipliers[1] = 0.0f;
    userData->multipliers[2] = 0.0f;
    userData->multipliers[3] = 0.0f;
    userData->multipliers[4] = 1.0f;
    userData->multipliers[5] = 0.0f;
    userData->multipliers[6] = 0.0f;
    userData->multipliers[7] = 0.0f;
    userData->multipliers[8] = 1.0f;

    EngineArguments *arguments = filterRequest->arguments;
    char *r1 = argz_next(arguments->value, arguments->length, NULL);
    char *r2 = r1 ? argz_next(arguments->value, arguments->length, r1) : NULL;
    char *r3 = r2 ? argz_next(arguments->value, arguments->length, r2) : NULL;
    char *g1 = r3 ? argz_next(arguments->value, arguments->length, r3) : NULL;
    char *g2 = g1 ? argz_next(arguments->value, arguments->length, g1) : NULL;
    char *g3 = g2 ? argz_next(arguments->value, arguments->length, g2) : NULL;
    char *b1 = g3 ? argz_next(arguments->value, arguments->length, g3) : NULL;
    char *b2 = b1 ? argz_next(arguments->value, arguments->length, b1) : NULL;
    char *b3 = b2 ? argz_next(arguments->value, arguments->length, b2) : NULL;

    if (r1)
        userData->multipliers[0] = atof(r1);
    if (r2)
        userData->multipliers[1] = atof(r2);
    if (r3)
        userData->multipliers[2] = atof(r3);
    if (g1)
        userData->multipliers[3] = atof(g1);
    if (g2)
        userData->multipliers[4] = atof(g2);
    if (g3)
        userData->multipliers[5] = atof(g3);
    if (b1)
        userData->multipliers[6] = atof(b1);
    if (b2)
        userData->multipliers[7] = atof(b2);
    if (b3)
        userData->multipliers[8] = atof(b3);

    configurations->userData = userData;
    configurations->freeUserData = FreeUserData;

    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    MultiplierUserData *multiplierUserData = (MultiplierUserData *)userData;
    float *multipliers = multiplierUserData->multipliers;
    
    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;

    for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
    {
        unsigned char sourceRed = sourceData[index * 3 + 0];
        unsigned char sourceGreen = sourceData[index * 3 + 1];
        unsigned char sourceBlue = sourceData[index * 3 + 2];

        float targetRed = sourceRed * multipliers[0] + sourceGreen * multipliers[1] + sourceBlue * multipliers[2];
        float targetGreen = sourceRed * multipliers[3] + sourceGreen * multipliers[4] + sourceBlue * multipliers[5];
        float targetBlue = sourceRed * multipliers[6] + sourceGreen * multipliers[7] + sourceBlue * multipliers[8];

        if (targetRed > 255.0f)
            targetRed = 255.0f;
        
        if (targetGreen > 255.0f)
            targetGreen = 255.0f;

        if (targetBlue > 255.0f)
            targetBlue = 255.0f;

        targetData[index * 3 + 0] = (unsigned char)targetRed;
        targetData[index * 3 + 1] = (unsigned char)targetGreen;
        targetData[index * 3 + 2] = (unsigned char)targetBlue;
    }

    return 0;
}

static void FreeUserData(void *userData)
{
    free(userData);
}