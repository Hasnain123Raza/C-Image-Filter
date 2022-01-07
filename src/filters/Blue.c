#include "Blue.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);

Filter Blue = {
    .filterId = FILTER_BLUE,
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
    memset(monochromeFilterRequest, 0, sizeof(FilterRequest));

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
    char *alphaArgument = argz_next(filterRequest->arguments->value, filterRequest->arguments->length, NULL);
    float alphaValue = 1.0f;
    if (alphaArgument)
        alphaValue = atof(alphaArgument);
    alphaValue = alphaValue < 0.0f ? 0.0f : alphaValue > 1.0f ? 1.0f : alphaValue;
    float multiplierValue = 1.0f - alphaValue;
    int multiplierValueLength = snprintf(NULL, 0, "%f", multiplierValue);
    char multiplierBuffer[multiplierValueLength + 1];
    snprintf(multiplierBuffer, multiplierValueLength + 1, "%f", multiplierValue);
    argz_add(&monochromeArguments->value, &monochromeArguments->length, multiplierBuffer);
    argz_add(&monochromeArguments->value, &monochromeArguments->length, multiplierBuffer);
    argz_add(&monochromeArguments->value, &monochromeArguments->length, "1.0");
    monochromeFilterRequest->arguments = monochromeArguments;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    return 0;
}