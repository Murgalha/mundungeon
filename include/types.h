#ifndef _TYPES_H_
#define _TYPES_H_

struct V2 {
	union {
		short x, col;
	};
	union {
		short y, row;
	};
};

struct V2f {
	union {
		float x, col;
	};
	union {
		float y, row;
	};
};

enum Direction {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

#endif
