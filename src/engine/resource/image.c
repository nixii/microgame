
#include "microgame/engine/resource/image.h"
#include <stdio.h>

// stob image type limiting
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG

// I use stb_image for loading images withotu having to code that myself.
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

// load an image from a path
image_resource image_resource_from(const char *path) {

    // create the structure
    image_resource r = { 0 };

    // load the image data
    int _; // throw-away
    unsigned char *pixelBytes = stbi_load(path, &r.width, &r.height, &_, 4);

    // error out
    if (pixelBytes == NULL) {
        fprintf(stderr, "image %s failed to load; %s\n", path, stbi_failure_reason());
        return r;
    }

    // the colors it will be stored as
    color *pixels = malloc(sizeof(color) * r.width * r.height);
    
    // load the colors in
    for (int i = 0; i < r.width * r.height * 4 * sizeof(unsigned char); i += 4) {
        pixels[i / 4] = rgba(pixelBytes[i], pixelBytes[i + 1], pixelBytes[i + 2], pixelBytes[i + 3]);
    }

    // free the image from stb_image
    stbi_image_free(pixelBytes);
    
    // set the pixels
    r.pixels = pixels;
    
    // all done!
    return r;
}

// destroy an image resource
void image_resource_destroy(image_resource *r) {
    free(r->pixels);
    r->pixels = NULL;
    r->width = 0;
    r->height = 0;
}