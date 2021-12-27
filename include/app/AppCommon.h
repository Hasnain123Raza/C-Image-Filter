#ifndef APP_COMMON_H
#define APP_COMMON_H

#include <stddef.h>

typedef struct {
    char *value;
    size_t length;
} Arguments;

typedef struct {
    char *inputImageFilename;
    char *outputImageFilename;
    int filterId;
    Arguments arguments;
} Configurations;

#endif