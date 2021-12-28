#include "Yellow.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);

Filter Yellow = {
    .filterId = FILTER_YELLOW,
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

    EngineArguments *arguments = (EngineArguments *)malloc(sizeof(EngineArguments));
    if (!arguments)
    {
        free(prefilters);
        free(monochromeFilterRequest);
        return 1;
    }
    arguments->length = 0;
    arguments->value = NULL;
    argz_add(&arguments->value, &arguments->length, "1.0");
    argz_add(&arguments->value, &arguments->length, "1.0");
    argz_add(&arguments->value, &arguments->length, "0.0");
    monochromeFilterRequest->arguments = arguments;

    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    return 0;
}