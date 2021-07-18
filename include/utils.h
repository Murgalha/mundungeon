#ifndef _UTILS_H_
#define _UTILS_H_

#include "types.h"

typedef struct {
	union {
		i16 x, col;
	};
	union {
		i16 y, row;
	};
} V2;

typedef struct {
	union {
		f32 x, col;
	};
	union {
		f32 y, row;
	};
} V2f;

#endif
