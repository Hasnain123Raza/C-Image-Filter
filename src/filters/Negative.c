#include "Negative.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);

Filter Negative = {
    .filterId = FILTER_NEGATIVE,
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

    FilterRequest *monochromeFilterRequest = (FilterRequest *)malloc(sizeof(FilterRequest));
    if (!monochromeFilterRequest)
    {
        free(prefilters);
        return 1;
    }
    prefilters[0] = monochromeFilterRequest;
    monochromeFilterRequest->filterId = FILTER_MONOCHROME;

    EngineArguments *monochromeArguments = (EngineArguments *)malloc(sizeof(EngineArguments));
    if (!monochromeArguments)
    {
        free(prefilters);
        free(monochromeFilterRequest);
        return 1;
    }
    monochromeArguments->length = 0;
    monochromeArguments->value = NULL;
    argz_add(&monochromeArguments->value, &monochromeArguments->length, "-1.0");
    argz_add(&monochromeArguments->value, &monochromeArguments->length, "-1.0");
    argz_add(&monochromeArguments->value, &monochromeArguments->length, "-1.0");
    monochromeFilterRequest->arguments = monochromeArguments;

    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    return 0;
}