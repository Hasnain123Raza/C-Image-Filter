#include "Filters.h"

Filter *getFilterFromFilterId(FilterID filterId)
{
    Filter *filter = NULL;

    switch (filterId)
    {
        case FILTER_NONE:
            filter = &None;
            break;
        case FILTER_MULTIPLIER:
            filter = &Multiplier;
            break;
        case FILTER_MONOCHROME:
            filter = &Monochrome;
            break;
        case FILTER_RED:
            filter = &Red;
            break;
        case FILTER_GREEN:
            filter = &Green;
            break;
        case FILTER_BLUE:
            filter = &Blue;
            break;
        case FILTER_YELLOW:
            filter = &Yellow;
            break;
        case FILTER_CYAN:
            filter = &Cyan;
            break;
        case FILTER_MAGENTA:
            filter = &Magenta;
            break;
        case FILTER_NEGATIVE:
            filter = &Negative;
            break;
        case FILTER_GRAYSCALE:
            filter = &Grayscale;
            break;
        case FILTER_BRIGHTNESS:
            filter = &Brightness;
            break;
        default:
            break;
    }

    return filter;
}