#include "Monochrome.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);

Filter Monochrome = {
    .filterId = FILTER_MONOCHROME,
    .configurator = Configurator,
    .function = NULL,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    EngineArguments *arguments = filterRequest->arguments;
    char *redModifierArgument = argz_next(arguments->value, arguments->length, NULL);
    char *greenModifierArgument = redModifierArgument ? argz_next(arguments->value, arguments->length, redModifierArgument) : NULL;
    char *blueModifierArgument = greenModifierArgument ? argz_next(arguments->value, arguments->length, greenModifierArgument) : NULL;

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
    argz_add(&multiplierArguments->value, &multiplierArguments->length, redModifierArgument ? redModifierArgument : "1.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, greenModifierArgument ? greenModifierArgument : "1.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.0");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, blueModifierArgument ? blueModifierArgument : "1.0");
    multiplierFilterRequest->arguments = multiplierArguments;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    return 0;
}