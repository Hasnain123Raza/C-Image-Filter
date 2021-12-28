#include "Monochrome.h"

typedef struct {
    float redModifier;
    float greenModifier;
    float blueModifier;
} MonochromeUserData;

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);
static void FreeUserData(void *userData);

Filter Monochrome = {
    .filterId = FILTER_MONOCHROME,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    MonochromeUserData *userData = (MonochromeUserData *)malloc(sizeof(MonochromeUserData));
    if (!userData)
        return 1;
    userData->redModifier = 1.0f;
    userData->greenModifier = 1.0f;
    userData->blueModifier = 1.0f;

    EngineArguments *arguments = filterRequest->arguments;
    char *redModifierArgument = argz_next(arguments->value, arguments->length, NULL);
    char *greenModifierArgument = redModifierArgument ? argz_next(arguments->value, arguments->length, redModifierArgument) : NULL;
    char *blueModifierArgument = greenModifierArgument ? argz_next(arguments->value, arguments->length, greenModifierArgument) : NULL;

    if (redModifierArgument)
        userData->redModifier = atof(redModifierArgument);

    if (greenModifierArgument)
        userData->greenModifier = atof(greenModifierArgument);

    if (blueModifierArgument)
        userData->blueModifier = atof(blueModifierArgument);

    configurations->userData = userData;
    configurations->freeUserData = FreeUserData;

    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    MonochromeUserData *monochromeUserData = (MonochromeUserData *)userData;
    
    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;

    for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
    {
        targetData[index * 3 + 0] = sourceData[index * 3 + 0] * monochromeUserData->redModifier;
        targetData[index * 3 + 1] = sourceData[index * 3 + 1] * monochromeUserData->greenModifier;
        targetData[index * 3 + 2] = sourceData[index * 3 + 2] * monochromeUserData->blueModifier;
    }

    return 0;
}

static void FreeUserData(void *userData)
{
    free(userData);
}