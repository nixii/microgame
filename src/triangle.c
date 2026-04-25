
#include "triangle.h"

float triangle_area(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return (x1 * (y2-y3) + x2 * (y3-y1) + x3 * (y1-y2))/2.0;
}