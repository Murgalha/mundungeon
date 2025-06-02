#ifndef _RECT_H_
#define _RECT_H_

#include "shapes/point.h"

typedef struct Rect {
    Point start_point;
	int width, height;
} Rect;

#endif
