#include "None.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(Chunk *chunk, void *userData);

Filter None = {
    .filterId = FILTER_NONE,
    .configurator = NULL,
    .function = NULL,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    return 0;
}

static int Function(Chunk *chunk, void *userData)
{
    return 0;
}