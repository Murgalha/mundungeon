#include <stdlib.h>
#include "random.h"

static u64 x, y, z;

void random_init(u32 seed) {
	srand(seed);
	x = rand();
	y = rand();
	z = rand();
}

u64 random() {
	u64 t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

u64 random_rangei(u64 begin, u64 end) {
	return (random() % (end - begin)) + begin;
}
