#include "Filters.h"

Filter *getFilterFromFilterId(FilterID filterId)
{
    Filter *filter = NULL;

    switch (filterId)
    {
        case FILTER_NONE:
            filter = &None;
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
        default:
            break;
    }

    return filter;
}