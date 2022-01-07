#include "Monochrome.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);
static void FreeUserData(void *userData);

Filter Monochrome = {
    .filterId = FILTER_MONOCHROME,
    .configurator = Configurator,
    .function = NULL,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    float redModifier = 1.0f;
    float greenModifier = 1.0f;
    float blueModifier = 1.0f;

    EngineArguments *arguments = filterRequest->arguments;
    char *redModifierArgument = argz_next(arguments->value, arguments->length, NULL);
    char *greenModifierArgument = redModifierArgument ? argz_next(arguments->value, arguments->length, redModifierArgument) : NULL;
    char *blueModifierArgument = greenModifierArgument ? argz_next(arguments->value, arguments->length, greenModifierArgument) : NULL;

    if (redModifierArgument)
        redModifier = atof(redModifierArgument);

    if (greenModifierArgument)
        greenModifier = atof(greenModifierArgument);

    if (blueModifierArgument)
        blueModifier = atof(blueModifierArgument);

    FilterRequest **prefilters = (FilterRequest **)malloc(sizeof(FilterRequest *) * 2);
    if (!prefilters)
        return 1;
    configurations->prefilters = prefilters;
    prefilters[0] = NULL;
    prefilters[1] = NULL;

    FilterRequest *multiplierFilterRequest = (FilterRequest *)malloc(sizeof(FilterRequest));
    if (!multiplierFilterRequest)
    {
        free(prefilters);
        return 1;
    }
    memset(multiplierFilterRequest, 0, sizeof(FilterRequest));

    prefilters[0] = multiplierFilterRequest;
    multiplierFilterRequest->filterId = FILTER_MULTIPLIER;

    EngineArguments *multiplierArguments = (EngineArguments *)malloc(sizeof(EngineArguments));
    if (!multiplierArguments)
    {
        free(prefilters);
        free(multiplierFilterRequest);
        return 1;
    }
    multiplierArguments->length = 0;
    multiplierArguments->value = NULL;
    int redBufferLength = snprintf(NULL, 0, "%f", redModifier);
    int greenBufferLength = snprintf(NULL, 0, "%f", greenModifier);
    int blueBufferLength = snprintf(NULL, 0, "%f", blueModifier);
    char redBuffer[redBufferLength + 1];
    char greenBuffer[greenBufferLength + 1];
    char blueBuffer[blueBufferLength + 1];
    snprintf(redBuffer, redBufferLength + 1, "%f", redModifier);
    snprintf(greenBuffer, greenBufferLength + 1, "%f", greenModifier);
    snprintf(blueBuffer, blueBufferLength + 1, "%f", blueModifier);
    argz_add(&multiplierArguments->value, &multiplierArguments->length, redBuffer);
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, greenBuffer);
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, blueBuffer);
    multiplierFilterRequest->arguments = multiplierArguments;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    return 0;
}