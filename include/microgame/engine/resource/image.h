
#ifndef MG_IMAGE_RESOURCE_H
#define MG_IMAGE_RESOURCE_H

#include "microgame/util/color.h"

// the base structure of an image resource
typedef struct image_resource {
    int width, height;
    color *pixels;
} image_resource;

/*
 * image resource functions
*/

// load an image from a path
image_resource image_resource_from(const char *path);

// destroy an image resource
void image_resource_destroy(image_resource *r);

#endif // MG_IMAGE_RESOURCE_H