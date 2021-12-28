#include "Sepia.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);

Filter Sepia = {
    .filterId = FILTER_SEPIA,
    .configurator = Configurator,
    .function = NULL,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
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
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.393");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.769");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.189");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.349");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.686");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.168");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.272");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.534");
    argz_add(&multiplierArguments->value, &multiplierArguments->length, "0.131");
    multiplierFilterRequest->arguments = multiplierArguments;

    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    return 0;
}