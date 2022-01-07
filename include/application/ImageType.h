#ifndef IMAGE_TYPE_H
#define IMAGE_TYPE_H

#include <string.h>

typedef enum {
    IMAGE_TYPE_UNKNOWN,
    IMAGE_TYPE_JPEG,
    IMAGE_TYPE_PNG
} ImageType;

ImageType getImageTypeFromFilename(char *filename);

#endif