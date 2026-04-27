
#include "microgame/util/triangle.h"

// compare triangles by depth
int triangle_cmp(const void *_a, const void *_b) {
    const triangle *a = (const triangle *)_a;
    const triangle *b = (const triangle *)_b;
    return (b->depth > a->depth) - (b->depth < a->depth);
}