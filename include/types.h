#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>
#include <stdbool.h>

// Signed
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

// Unsigned
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

// Floating point
#define f32 float
#define f64 double

struct V2 {
	union {
		i16 x, col;
	};
	union {
		i16 y, row;
	};
};

struct V2f {
	union {
		f32 x, col;
	};
	union {
		f32 y, row;
	};
};

enum Direction {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

#endif
